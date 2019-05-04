#ifndef XERXES_TEARDROP_H
#define XERXES_TEARDROP_H

#include "spoofed_udp_flood.h"

class teardrop : public spoofed_udp_flood {

public:
  teardrop();
  ~teardrop() = default;

protected:
  void finalize_hdr(udphdr *udp, iphdr *ip) override;
};

#endif // XERXES_TEARDROP_H
