#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H

#include "base_spoofed_flood.h"
#include <netinet/tcp.h>

class spoofed_tcp_flood : public base_spoofed_flood {

public:
    explicit spoofed_tcp_flood(std::shared_ptr<Config> config);
    ~spoofed_tcp_flood() override = default;

protected:
    char *gen_hdr(sockaddr_in *dst, int len) override;


    struct pshdr{
        u_int32_t saddr = 0;
        u_int32_t daddr = 0;
        u_int8_t zero = 0;
        u_int8_t proto = IPPROTO_TCP;
        u_int16_t len = 0;
        tcphdr tcp;
    };
};


#endif //XERXES_SPOOFED_TCP_FLOOD_H
