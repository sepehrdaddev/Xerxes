#ifndef XERXES_CONFIGURATION_H
#define XERXES_CONFIGURATION_H

#include <string>
#include <vector>

#include "Version.hpp"

struct config{
    enum Vector{NullTCP, NullUDP, TCPFlood, UDPFlood, HTTP, Slowloris, ICMPFlood, SpoofedTCP, SpoofedUDP, Rudy, Blacknurse, SSL_Flood};
    enum Protocol{TCP, UDP};
    Protocol protocol{TCP};
    Vector vector{NullTCP};
    std::string website{};
    std::string port{"1"};
    std::vector<std::string> useragents{"Wget/1.16 (linux-gnu/Xerxes)"};
    int THREADS{10};
    int CONNECTIONS{10};
    bool GetResponse{false};
    bool RandomizeUserAgent{false};
    bool RandomizeHeader{false};
    bool UseSSL{false};
    int delay{3000};

};

const char Version[] = {
        BUILD_YEAR_CH0, BUILD_YEAR_CH1,
        BUILD_YEAR_CH2, BUILD_YEAR_CH3,
        '.', BUILD_MONTH_CH0, BUILD_MONTH_CH1,
        '.', BUILD_DAY_CH0, BUILD_DAY_CH1, '\0'
};

#endif //XERXES_CONFIGURATION_H
