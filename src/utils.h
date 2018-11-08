#ifndef XERXES_UTILS_H
#define XERXES_UTILS_H

#include <string>
#include <random>
#include <chrono>

namespace utils {
    static int randomInt(int min, int max){
        static std::mt19937 engine{};
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }
    static void randomIP(std::string& src){
        src.clear();
        src = std::to_string(randomInt(1, 256))
                + "."
                + std::to_string(randomInt(1, 256))
                + "."
                +  std::to_string(randomInt(1, 256))
                + "."
                + std::to_string(randomInt(1, 256));
    }
    static int randomPort(){
        static auto init_s_port = 0;
        static int seq = 0;
        if(init_s_port == 0){
            init_s_port = 1024 + (randomInt(0, 2000));
            return init_s_port;
        }
        ++seq;
        return (seq + init_s_port) % 65536;
    }
    static void randomstr(std::string& src){
        static int string_length = randomInt(0, 30);
        src.reserve(string_length);
        for(int i = 0; i < string_length; ++i){
            src += (static_cast<char>('0' + randomInt(0, 72)));
        }
    }

};


#endif //XERXES_UTILS_H
