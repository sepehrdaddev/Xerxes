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

    static const void randomIP(std::string& src){
        src = std::to_string(randomInt(1, 256));
        src += "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256))
               + "."
               + std::to_string(randomInt(1, 256));
    }

    static int randomPort(){
        return randomInt(0, 65535);
    }

    static const void randomstr(std::string& src){
        int string_length =  randomInt(0, 20);
        for(int i = 0; i < string_length; i++){
            src += (static_cast<char>('0' + randomInt(0, 72)));
        }
    }

    static const std::string& randomize_Vector(const std::vector<std::string> &vec){
        return vec[randomInt(0, static_cast<int>(vec.size()) -1)];
    }

    static const std::string& random_referer(){
        static std::vector<std::string> referer{
            "https://www.google.com/", "https://www.yahoo.com/", "https://www.bing.com/",
            "https://twitter.com/", "https://www.facebook.com/", "https://www.msn.com/",
            "https://www.youtube.com/", "https://yandex.com/", "https://www.amazon.com/"};
        return randomize_Vector(referer);
    }

    static const std::string& random_useragent(const std::vector<std::string> &useragents){
        return randomize_Vector(useragents);
    }

    static const std::string& random_encoding(){
        static std::vector<std::string> encoding{"\'\'", "*", "identity", "gzip", "deflate"};
        return randomize_Vector(encoding);
    }

    static const std::string& random_caching(){
        static std::vector<std::string> caching{"no-cache", "max-age=0"};
        return randomize_Vector(caching);
    }

    static const std::string& random_charset(){
        static std::vector<std::string> charset{"ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
        return randomize_Vector(charset);
    }

    static const std::string& random_contenttype(){
        static std::vector<std::string> contenttype{"multipart/form-data", "application/x-url-encoded"};
        return randomize_Vector(contenttype);
    }

    static const std::string& random_method(){
        static std::vector<std::string> methods{"GET", "HEAD"};
        return randomize_Vector(methods);
    }
}

#endif //XERXES_RANDOMIZER_H
