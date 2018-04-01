#ifndef XERXES_CONFIGURATION_H
#define XERXES_CONFIGURATION_H

#include <string>
#include <vector>

struct config{
    enum Vector{NullTCP, NullUDP, TCPFlood, UDPFlood, HTTP, Slowloris, ICMPFlood, SpoofedTCP, SpoofedUDP};
    enum Protocol{TCP, UDP};
    Protocol protocol{TCP};
    Vector vector{NullTCP};
    std::string website{};
    std::string port{};
    std::vector<std::string> useragents{"Wget/1.16 (linux-gnu/Xerxes)"};
    int THREADS{0};
    int CONNECTIONS{0};
    bool GetResponse{false};
    bool RandomizeUserAgent{false};
    bool RandomizeHeader{false};
    bool UseSSL{false};
    int delay{3000};

};

#endif //XERXES_CONFIGURATION_H
