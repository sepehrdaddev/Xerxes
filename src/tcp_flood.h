#ifndef XERXES_TCP_FLOOD_H
#define XERXES_TCP_FLOOD_H

#include <memory>
#include <string>

#include "base_flood.h"
#include "config.h"

class tcp_flood : public base_flood {

public:
    explicit tcp_flood(std::shared_ptr<Config> config);
    ~tcp_flood() override = default;
};


#endif //XERXES_TCP_FLOOD_H
