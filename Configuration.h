#ifndef XERXES_CONFIGURATION_H
#define XERXES_CONFIGURATION_H

#include <string>
#include <vector>

struct config{
    enum Protocol{TCP, UDP};
    enum Vector{Null, NullPlus, HTTP, HTTPPlus};
    Protocol protocol{TCP};
    Vector vector{Null};
    std::string website{};
    std::string port{};
    std::vector<std::string> useragents{"Wget/1.16 (linux-gnu/Xerxes)"};
    int THREADS = 0;
    int CONNECTIONS = 0;

};

#endif //XERXES_CONFIGURATION_H
