#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H


#include "Spoofed_Flood.hpp"

class Spoofed_TCP_Flood : public Spoofed_Flood {
public:
    Spoofed_TCP_Flood(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;
    struct tcpheader {
        unsigned short int tcp_srcport;
        unsigned short int tcp_destport;
        unsigned int       tcp_seqnum;
        unsigned int       tcp_acknum;
        unsigned char      tcp_reserved:4, tcp_offset:4;
        unsigned int
                tcp_res1:4,
                tcp_hlen:4,
                tcp_fin:1,
                tcp_syn:1,
                tcp_rst:1,
                tcp_psh:1,
                tcp_ack:1,
                tcp_urg:1,
                tcp_res2:2;
        unsigned short int tcp_win;
        unsigned short int tcp_chksum;
        unsigned short int tcp_urgptr;
    };
};


#endif //XERXES_SPOOFED_TCP_FLOOD_H
