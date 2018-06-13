#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <sys/ioctl.h>

#include "../Headers/Server.hpp"
#include "../Headers/Logging.hpp"
#include "../Headers/Randomizer.hpp"

std::vector<Client *> Server::clients;
std::vector<ServerThread *> Server::threads;
configuration *Server::config;

Server::Server(int port) {
   config = new configuration{0, port, "cert.pem", "key.pem", false};
   ServerThread::InitMutex();
   init_openssl();
   config->ctx = InitCTX();
   check_certs();
   configure_CTX(config->ctx);
   config->socket = make_socket(config->port);
   clients.reserve(config->maxClients);
   threads.reserve(config->maxClients);
}

Server::~Server(){
    for(auto &thread : threads){
        delete thread;
    }
    for(auto &client : clients){
        delete client;
    }
    close(config->socket);
    SSL_CTX_free(config->ctx);
    cleanup_openssl();
    delete config;
}

void Server::Serve() {
    config->running = true;

    auto *cmd = new ServerThread{};
    cmd->Create(reinterpret_cast<void *>(Server::command), nullptr);

    auto *handler = new ServerThread{};
    handler->Create(reinterpret_cast<void *>(Server::Accept_Clients), nullptr);

    cmd->Join();

    delete cmd;
    delete handler;

}

int Server::make_socket(int &port) {
    int s;
    struct sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        print_error("Unable to create socket");
        exit(EXIT_FAILURE);
    }
    int on = 1;
    setsockopt(s,SOL_SOCKET,SO_REUSEADDR, &on,sizeof(int));
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(int));

    if (bind(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        print_error("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 5) < 0) {
        print_error("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return s;
}

int Server::read_socket(SSL *sock, std::string *buff) {
    return SSL_read(sock, const_cast<char *>(buff->c_str()), 256);
}

int Server::write_socket(SSL *sock, std::string *buff) {
    return SSL_write(sock, buff->c_str(), static_cast<int>(buff->length()));
}

EVP_PKEY *Server::generate_key() {
    EVP_PKEY * pkey = EVP_PKEY_new();
    if(!pkey) {
        print_error("Unable to create EVP_PKEY structure.");
        return nullptr;
    }
    RSA *rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);

    if(!EVP_PKEY_assign_RSA(pkey, rsa)) {
        print_error("Unable to generate 2048-bit RSA key.");
        EVP_PKEY_free(pkey);
        return nullptr;
    }

    return pkey;
}

X509 *Server::generate_x509(EVP_PKEY *pkey) {
    X509 * x509 = X509_new();
    if(!x509) {
        print_error( "Unable to create X509 structure.");
        return nullptr;
    }

    /* Set the serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    /* This certificate is valid from now until exactly one year from now. */
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

    /* Set the public key for our certificate. */
    X509_set_pubkey(x509, pkey);

    /* We want to copy the subject name to the issuer name. */
    X509_NAME * name = X509_get_subject_name(x509);

    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)"CA", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)"Company", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);

    /* Now set the issuer name. */
    X509_set_issuer_name(x509, name);

    /* Actually sign the certificate with our key. */
    if(!X509_sign(x509, pkey, EVP_sha1())) {
        print_error("Error signing certificate." );
        X509_free(x509);
        return nullptr;
    }

    return x509;
}

bool Server::save_key(EVP_PKEY *pkey, X509 *x509) {
    FILE * pkey_file = fopen(config->key_file, "wb");
    if(!pkey_file)
    {
        print_error("Unable to open key file for writing.");
        return false;
    }

    /* Write the key to disk. */
    int ret = PEM_write_PrivateKey(pkey_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(pkey_file);

    if(!ret) {
        print_error("Unable to write private key to disk.");
        return false;
    }

    /* Open the PEM file for writing the certificate to disk. */
    FILE * x509_file = fopen(config->cert_file, "wb");
    if(!x509_file)
    {
        print_error("Unable to open cert file for writing.");
        return false;
    }

    /* Write the certificate to disk. */
    ret = PEM_write_X509(x509_file, x509);
    fclose(x509_file);

    if(!ret)
    {
        print_error("Unable to write certificate to disk.");
        return false;
    }

    return true;
}

void Server::generate_certificates() {
    EVP_PKEY * pkey = generate_key();
    if(!pkey){
        exit(EXIT_FAILURE);
    }

    X509 * x509 = generate_x509(pkey);
    if(!x509){
        EVP_PKEY_free(pkey);
        exit(EXIT_FAILURE);
    }

    bool ret = save_key(pkey, x509);
    EVP_PKEY_free(pkey);
    X509_free(x509);

    if(!ret){
        print_error("Unable to generate certificate.");
        exit(EXIT_FAILURE);
    }
}

void Server::init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void Server::cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX *Server::InitCTX() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx){
        print_error("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void Server::configure_CTX(SSL_CTX *ctx) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0){
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ){
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        print_error("Private key does not match the public certificate");
        exit(EXIT_FAILURE);
    }
}

void Server::check_certs() {
    FILE *key = fopen(config->key_file, "rb");
    FILE *cert = fopen(config->cert_file, "rb");
    if((!key) || (!cert)){
        generate_certificates();
    }
}

void *Server::HandleClient(void *arg) {
    auto *client = reinterpret_cast<Client *>(arg);
    std::string buff{"Connected..."};
    ServerThread::LockMutex();

    client->id = static_cast<int>(clients.size());
    clients.emplace_back(client);

    ServerThread::UnlockMutex();

    write_socket(client->ssl, &buff);
    buff.clear();

    while(config->running){
        if(is_connected(client->ssl)){
            int c = 0;
            ioctl(client->socket, FIONREAD, &c);
            if(c > 0){
                if(read_socket(client->ssl, &buff) == -1){
                    print_error("Error while receiving from client");
                }
            }
        }else{
            printf("Client %d diconnected", client->id);

            ServerThread::LockMutex();

            for(size_t i = 0; i < clients.size(); i++) {
                if((Server::clients[i]->id) == client->id){
                    delete client;
                    Server::clients.erase(Server::clients.begin() + i);
                }
            }

            ServerThread::UnlockMutex();
            break;
        }
        usleep(5000000);
    }

    return nullptr;
}

void *Server::command(void *) {

    std::string cmd{};

    while(config->running){
        printf("> ");
        std::getline(std::cin, cmd);
        if(cmd == "exit"){
            config->running = false;
            exit(EXIT_SUCCESS);
        }else if(!cmd.empty()){
            SendToAll(&cmd);
        }
    }
    return nullptr;
}

void Server::SendToAll(std::string *buff) {
    //Acquire the lock
    ServerThread::LockMutex();

    for (auto &client : clients) {
        if(is_connected(client->ssl)){
            write_socket(client->ssl, buff);
        }
    }

    buff->clear();

    //Release the lock
    ServerThread::UnlockMutex();
}

void *Server::Accept_Clients(void *) {
    Client *client;
    ServerThread *thread;

    while(config->running){
        if(clients.size() <= config->maxClients){
            client = new Client{};
            struct sockaddr_in addr{};
            uint len = sizeof(addr);
            client->socket = accept(config->socket, reinterpret_cast<sockaddr*>(&addr), &len);
            if (client->socket < 0){
                print_error("Unable to accept");
                delete client;
            }else{
                config->clients++;
                printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                client->ssl = SSL_new(config->ctx);
                SSL_set_fd(client->ssl, client->socket);

                if (SSL_accept(client->ssl) <= 0){
                    ERR_print_errors_fp(stderr);
                }else{
                    thread = new ServerThread{};
                    thread->Create(reinterpret_cast<void *>(Server::HandleClient), client);
                    threads.emplace_back(thread);
                }
            }
        }
    }
    return nullptr;
}

bool Server::is_connected(SSL *ssl) {
    char data[] = "\0";
    auto rc = SSL_write(ssl, reinterpret_cast<void *>(data), 1);
    return rc > 0;
}
