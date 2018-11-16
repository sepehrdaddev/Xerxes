#ifndef XERXES_LAND_H
#define XERXES_LAND_H

#include "spoofed_tcp_flood.h"

class land : public spoofed_tcp_flood{

public:
    explicit land(std::shared_ptr<Config> config);
    ~land() override = default;

protected:
    void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};


#endif //XERXES_LAND_H
