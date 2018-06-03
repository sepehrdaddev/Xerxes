#ifndef XERXES_CONFIGURATION_H
#define XERXES_CONFIGURATION_H

#include <string>
#include <vector>
#include <memory>
#include <ctime>

#include "Version.hpp"

struct Config{
    enum Vector{NullTCP, NullUDP, TCPFlood, UDPFlood, HTTP, Slowloris, ICMPFlood, SpoofedUDP,
        SpoofedSyn, SpoofedAck, SpoofedRST, SpoofedPUSH, SpoofedURG, SpoofedFin, Rudy, Blacknurse,
        Beast, TearDrop, Land, Smurf};
    enum Protocol{TCP = 1, UDP};
    Protocol protocol{TCP};
    Vector vector{NullTCP};
    std::string website{};
    std::string port{"1"};
    std::string broadcast{};
    int THREADS{10};
    int CONNECTIONS{25};
    bool GetResponse{false};
    bool RandomizeUserAgent{false};
    bool RandomizeHeader{false};
    bool UseSSL{false};
    bool RandomizeSource{false};
    bool RandomizePort{false};
    int delay{0};
    std::vector<std::string> *useragents = nullptr;
    unsigned long long *voly = nullptr;
    unsigned long long *req = nullptr;
    struct Timer{
        clock_t start_time;
        clock_t end_time;
        clock_t get(){
            return end_time - start_time;
        }
    }timer{};
    void show_stat(){
        if((!website.empty()) && (*voly > 0) && (*req > 0)){
            timer.end_time = clock();
            fprintf(stdout, "--- %s Attack statistics ---\n%llu Voly sent, %llu Request sent, Time %f s\n",
                    website.c_str(), *voly, *req,((float)timer.get())/CLOCKS_PER_SEC);
        }
    }
    Config(){
        useragents = new std::vector<std::string>();
        voly = new unsigned long long(0);
        req = new unsigned long long(0);
    }
    ~Config(){
        delete useragents;
        delete voly;
        delete req;
    }
};

const char Version[] = {
        VERSION_MAJOR_INIT,
        '.',
        VERSION_MINOR_INIT,
        '\0'
};
const char Time[] = {
        BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
        BUILD_MONTH_CH0, BUILD_MONTH_CH1,
        BUILD_DAY_CH0, BUILD_DAY_CH1,
        '\0'
};

#endif //XERXES_CONFIGURATION_H
