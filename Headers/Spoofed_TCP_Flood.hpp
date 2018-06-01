#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H

#include "Spoofed_Flood.hpp"

class Spoofed_TCP_Flood : public Spoofed_Flood {
public:
    explicit Spoofed_TCP_Flood(std::shared_ptr<Config> conf);

private:
    void attack() override;
    void init_headers(iphdr *ip, tcphdr *tcp, char *buf);
    void override_headers(tcphdr *tcp, iphdr *ip);
};


#endif //XERXES_SPOOFED_TCP_FLOOD_H
