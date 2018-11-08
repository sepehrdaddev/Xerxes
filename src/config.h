#ifndef XERXES_CONFIG_H
#define XERXES_CONFIG_H

#include <string>

#define NULL_TCP (0)

struct Config{
    std::string rhost;
    std::string rport;
    std::string bcast;
    int vec;
    int trds;
    int conn;
    int dly;
    bool tls;
    bool rand_lhost;
    bool rand_lport;
    bool rand_usr;
    bool rand_hdr;
};

#endif //XERXES_CONFIG_H
