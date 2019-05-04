#ifndef XERXES_NULL_UDP_H
#define XERXES_NULL_UDP_H

#include "udp_flood.h"

class null_udp : public udp_flood {
public:
  null_udp();

  ~null_udp() = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_NULL_UDP_H
