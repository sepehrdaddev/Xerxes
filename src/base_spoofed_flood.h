#ifndef XERXES_BASE_SPOOFED_FLOOD_H
#define XERXES_BASE_SPOOFED_FLOOD_H

#include "Vector.h"
#include "rsocket.h"

class base_spoofed_flood : public Vector {
public:
    base_spoofed_flood(std::shared_ptr<Config> config, int protocol);
    ~base_spoofed_flood() override = default;
    void run() override;

protected:
    virtual char *gen_hdr(sockaddr_in *dst, int len) = 0;
    virtual void init_sockets(std::vector<std::unique_ptr<Rsocket>> &sockets);
    int proto{};
    int hdr_len{};
};


#endif //XERXES_BASE_SPOOFED_FLOOD_H
