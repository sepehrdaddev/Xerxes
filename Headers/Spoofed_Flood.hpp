#ifndef XERXES_SPOOFED_FLOOD_H
#define XERXES_SPOOFED_FLOOD_H

#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#include "Configuration.hpp"
#include "Logger.hpp"
#include "Attack_Vector.hpp"

class Spoofed_Flood : public Attack_Vector {

    friend class ICMP_Flood;
    friend class Spoofed_TCP_Flood;
    friend class Spoofed_UDP_Flood;
    friend class Black_Nurse;

public:
    Spoofed_Flood() = default;
    Spoofed_Flood(const config *conf, Logger *logger);

private:
    unsigned short csum(unsigned short *buf, int len);
    struct pseudo_header{
        u_int32_t source_address;
        u_int32_t dest_address;
        u_int8_t placeholder;
        u_int8_t protocol;
        u_int16_t length;
    };
    virtual int make_socket(int protocol);
    virtual void init_headers(iphdr *ip, tcphdr *tcp, char *buf);
    virtual void init_headers(iphdr *ip, udphdr *udp, char *buf);
    virtual void init_headers(iphdr *ip, icmphdr *icmp, char *buf);
};


#endif //XERXES_SPOOFED_FLOOD_H
