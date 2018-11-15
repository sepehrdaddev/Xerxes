#ifndef XERXES_SYN_FLOOD_H
#define XERXES_SYN_FLOOD_H

#include "spoofed_tcp_flood.h"

class syn_flood : public spoofed_tcp_flood {

public:
    explicit syn_flood(std::shared_ptr<Config> config);
    ~syn_flood() override = default;
};


#endif //XERXES_SYN_FLOOD_H
