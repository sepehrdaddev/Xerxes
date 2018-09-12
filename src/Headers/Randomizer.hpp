#ifndef XERXES_RANDOMIZER_H
#define XERXES_RANDOMIZER_H

#include <string>
#include <random>
#include <chrono>

#include "Configuration.hpp"

namespace Randomizer{

    static int randomInt(int min, int max){
        static unsigned seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
        static std::default_random_engine engine(seed);
        static std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }

    static void randomIP(std::string& src){
        std::snprintf(const_cast<char*>(src.c_str()), sizeof(int) * 4, "%d.%d.%d.%d", randomInt(1, 256)
                , randomInt(1, 256), randomInt(1, 256), randomInt(1, 256));
    }

    static int randomPort(){
        static auto init_s_port = 0;
        static int seq = 0;
        seq++;
        if(init_s_port == 0){
            init_s_port = 1024 + (randomInt(0, 2000));
            return init_s_port;
        }
        return (seq + init_s_port) % 65536;
    }

    static void randomstr(std::string& src){
        int string_length =  randomInt(0, 20);
        for(int i = 0; i < string_length; i++){
            src += (static_cast<char>('0' + randomInt(0, 72)));
        }
    }

    static void randomize_Vector(const std::vector<std::string> &vec, std::string& src){
        src = vec[randomInt(0, static_cast<int>(vec.size()) -1)];
    }

    static void random_referer(std::string& src){
        static std::vector<std::string> referer{
            "https://www.google.com/", "https://www.yahoo.com/", "https://www.bing.com/",
            "https://twitter.com/", "https://www.facebook.com/", "https://www.msn.com/",
            "https://www.youtube.com/", "https://yandex.com/", "https://www.amazon.com/"};
        randomize_Vector(referer, src);
    }

    static void random_useragent(const std::vector<std::string> &useragents, std::string& src){
        randomize_Vector(useragents, src);
    }

    static void random_encoding(std::string& src){
        static std::vector<std::string> encoding{"\'\'", "*", "identity", "gzip", "deflate"};
        randomize_Vector(encoding, src);
    }

    static void random_caching(std::string& src){
        static std::vector<std::string> caching{"no-cache", "max-age=0"};
        randomize_Vector(caching, src);
    }

    static void random_charset(std::string& src){
        static std::vector<std::string> charset{"ISO-8859-1", "utf-8", "Windows-1251", "ISO-8859-2", "ISO-8859-15"};
        randomize_Vector(charset, src);
    }

    static void random_contenttype(std::string& src){
        static std::vector<std::string> contenttype{"multipart/form-data", "application/x-url-encoded"};
        randomize_Vector(contenttype, src);
    }

    static void random_method(std::string& src){
        static std::vector<std::string> methods{"GET", "HEAD"};
        randomize_Vector(methods, src);
    }
}

#endif //XERXES_RANDOMIZER_H
