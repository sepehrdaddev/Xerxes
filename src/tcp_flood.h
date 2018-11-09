#ifndef XERXES_TCP_FLOOD_H
#define XERXES_TCP_FLOOD_H

#include <memory>
#include <string>

#include "config.h"

class tcp_flood {
public:
    explicit tcp_flood(std::shared_ptr<Config> config);
    virtual ~tcp_flood() = default;
    void run();

protected:
    virtual void gen_hdr(std::string& string);
    std::shared_ptr<Config> config;
};


#endif //XERXES_TCP_FLOOD_H
