#ifndef XERXES_SPOOFED_FLOOD_H
#define XERXES_SPOOFED_FLOOD_H


#include "Configuration.hpp"
#include "Logger.hpp"
#include "Attack_Vector.hpp"

class Spoofed_Flood : public Attack_Vector {

    friend class ICMP_Flood;
    friend class Spoofed_TCP_Flood;
    friend class Spoofed_UDP_Flood;

public:
    Spoofed_Flood() = default;
    Spoofed_Flood(const config *conf, Logger *logger);

private:
    unsigned short csum(unsigned short *buf, int len);
};


#endif //XERXES_SPOOFED_FLOOD_H
