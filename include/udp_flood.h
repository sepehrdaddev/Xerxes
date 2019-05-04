#ifndef XERXES_UDP_FLOOD_H
#define XERXES_UDP_FLOOD_H

#include "base_flood.h"

#include <arpa/inet.h>

class udp_flood : public base_flood {
public:
  udp_flood();
  ~udp_flood() = default;
};

#endif // XERXES_UDP_FLOOD_H
