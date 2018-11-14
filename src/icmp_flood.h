#ifndef XERXES_ICMP_FLOOD_H
#define XERXES_ICMP_FLOOD_H

#include "base_spoofed_flood.h"

class icmp_flood : public base_spoofed_flood {

public:
    explicit icmp_flood(std::shared_ptr<Config> config);
    ~icmp_flood() override = default;

protected:
    char *gen_hdr(sockaddr_in *dst, int len) override;
};


#endif //XERXES_ICMP_FLOOD_H
