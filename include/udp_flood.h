#ifndef XERXES_UDP_FLOOD_H
#define XERXES_UDP_FLOOD_H

#include "base_flood.h"
#include <arpa/inet.h>

class udp_flood : public base_flood {
public:
  explicit udp_flood(std::shared_ptr<Config> config);
  ~udp_flood() override = default;
};

#endif // XERXES_UDP_FLOOD_H
