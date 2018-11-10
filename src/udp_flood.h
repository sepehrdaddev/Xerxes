#ifndef XERXES_UDP_FLOOD_H
#define XERXES_UDP_FLOOD_H

#include "Vector.h"

class udp_flood : public Vector{
public:
    explicit udp_flood(std::shared_ptr<Config> config);
    ~udp_flood() override = default;
    void run() override;

protected:
    virtual int gen_hdr(std::string& string);
};


#endif //XERXES_UDP_FLOOD_H
