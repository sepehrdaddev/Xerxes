#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

#include "../Headers/Server.hpp"
#include "../Headers/Logging.hpp"

Server::Server(int port) {
   config = new configuration{0, port, "cert.pem", "key.pem", false};
}

Server::~Server(){
    delete config;
}

void Server::Serve() {
    int sock;
    SSL_CTX *ctx;

    init_openssl();
    ctx = InitCTX();
    check_certs();
    configure_CTX(ctx);

    sock = make_socket(config->port);

    std::string buff{"test"};

    /* Handle connections */
    while(config->running){
        struct sockaddr_in addr{};
        uint len = sizeof(addr);
        SSL *ssl;

        int client = accept(sock, reinterpret_cast<sockaddr*>(&addr), &len);
        if (client < 0){
            print_error("Unable to accept");
            exit(EXIT_FAILURE);
        }
        config->clients++;
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0){
            ERR_print_errors_fp(stderr);
        }else{
            write_socket(ssl, &buff);
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
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

    if (bind(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        print_error("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
        print_error("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return s;
}

void Server::read_socket(SSL *sock, std::string *buff) {
    while(SSL_read(sock, const_cast<char *>(buff->c_str()), 256)){}
}

void Server::write_socket(SSL *sock, std::string *buff) {
    SSL_write(sock, const_cast<char *>(buff->c_str()), static_cast<int>(buff->length()));
}

EVP_PKEY *Server::generate_key() {
    EVP_PKEY * pkey = EVP_PKEY_new();
    if(!pkey) {
        print_error("Unable to create EVP_PKEY structure.");
        return nullptr;
    }
    BIGNUM *e;
    e = BN_new();
    BN_set_word(e, 65537);
    RSA *rsa = nullptr;
    RSA_generate_key_ex(rsa, 2048, e, nullptr);
    if(!EVP_PKEY_assign_RSA(pkey, rsa)) {
        print_error("Unable to generate 2048-bit RSA key.");
        EVP_PKEY_free(pkey);
        return nullptr;
    }

    BN_free(e);

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
