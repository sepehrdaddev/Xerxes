#ifndef XERXES_TEARDROP_H
#define XERXES_TEARDROP_H

#include "spoofed_udp_flood.h"

class teardrop : public spoofed_udp_flood {

public:
  explicit teardrop(std::shared_ptr<Config> config);
  ~teardrop() override = default;

protected:
  void finalize_hdr(udphdr *udp, iphdr *ip) override;
};

#endif // XERXES_TEARDROP_H
