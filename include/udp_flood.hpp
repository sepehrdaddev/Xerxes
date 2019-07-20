#ifndef XERXES_UDP_FLOOD_H
#define XERXES_UDP_FLOOD_H

#include "base_flood.hpp"

#include <arpa/inet.h>

class udp_flood : public base_flood {
public:
  udp_flood();
  ~udp_flood() = default;
};

class null_udp : public udp_flood {
public:
  null_udp();

  ~null_udp() = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_UDP_FLOOD_H
