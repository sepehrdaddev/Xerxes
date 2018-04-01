#ifndef XERXES_SPOOFED_FLOOD_H
#define XERXES_SPOOFED_FLOOD_H


#include "Configuration.hpp"
#include "Logger.hpp"

class Spoofed_Flood {
    friend class ICMP_Flood;
    friend class Spoofed_TCP_Flood;
    friend class Spoofed_UDP_Flood;
public:
    Spoofed_Flood() = default;
    Spoofed_Flood(const config *conf, Logger *logger);
    void run();

private:
    virtual void attack(const int *id);
    unsigned short csum(unsigned short *buf, int len);
    const config *conf;
    Logger *logger;
};


#endif //XERXES_SPOOFED_FLOOD_H
