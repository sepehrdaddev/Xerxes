#ifndef XERXES_ICMP_FLOOD_H
#define XERXES_ICMP_FLOOD_H

#include "Spoofed_Flood.hpp"

class ICMP_Flood : public Spoofed_Flood {
public:
    ICMP_Flood(const Config *conf, Logger *logger);

private:
    void attack(const int *id) override;
    void init_headers(iphdr *ip, icmphdr *icmp, char *buf);
    void override_headers(icmphdr *tcp, iphdr *ip);

};


#endif //XERXES_ICMP_FLOOD_H
