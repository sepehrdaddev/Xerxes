#ifndef XERXES_UTILS_H
#define XERXES_UTILS_H

#include <string>
#include <random>

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
        src.clear();
        for(int i = 0; i < string_length; ++i){
            src += (static_cast<char>('0' + randomInt(0, 72)));
        }
    }

    static void pause(timespec time){
        nanosleep(&time, nullptr);
    }

    static void set_dly(int dly, timespec *time){
        if(time){
            if((dly - 999999999) > 0){
                time->tv_sec = dly - 999999999;
                time->tv_nsec = abs(1000000000 - dly);
            }else if (dly <= 0){
                time->tv_nsec = 1;
                time->tv_sec = 0;
            }else{
                time->tv_nsec = dly;
                time->tv_sec = 0;
            }
        }

    }

    static int to_int(const std::string& str){
        return static_cast<int>(strtol(str.c_str(), nullptr, 10));
    }

};


#endif //XERXES_UTILS_H
