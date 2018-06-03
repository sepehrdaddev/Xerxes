#ifndef XERXES_ICMP_FLOOD_H
#define XERXES_ICMP_FLOOD_H

#include <netinet/ip_icmp.h>

#include "Spoofed_Flood.hpp"

class ICMP_Flood : public Spoofed_Flood {
public:
    explicit ICMP_Flood(std::shared_ptr<Config> conf);

private:
    void attack() override;
    void init_headers(iphdr *ip, icmphdr *icmp, char *buf);
    void override_headers(icmphdr *tcp, iphdr *ip);

};


#endif //XERXES_ICMP_FLOOD_H
