#ifndef XERXES_CONFIG_H
#define XERXES_CONFIG_H

#include <string>

enum __Vector__{NULL_TCP = 0, NULL_UDP, TCP_FLOOD, UDP_FLOOD};

struct Config{
    std::string rhost;
    std::string rport;
    std::string bcast;
    int vec;
    int trds;
    int conn;
    bool tls;
    bool rand_lhost;
    bool rand_lport;
    bool rand_usr;
    bool rand_hdr;
    timespec time;
};

#endif //XERXES_CONFIG_H
