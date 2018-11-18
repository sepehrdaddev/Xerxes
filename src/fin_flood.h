#ifndef XERXES_FIN_FLOOD_H
#define XERXES_FIN_FLOOD_H

#include "spoofed_tcp_flood.h"

class fin_flood : public spoofed_tcp_flood{

public:
    explicit fin_flood(std::shared_ptr<Config> config);
    ~fin_flood() override = default;

protected:
    void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};


#endif //XERXES_FIN_FLOOD_H
