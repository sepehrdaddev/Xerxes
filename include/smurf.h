#ifndef XERXES_SMURF_H
#define XERXES_SMURF_H

#include "icmp_flood.h"

class smurf : public icmp_flood {

public:
  explicit smurf(std::shared_ptr<Config> config);
  ~smurf() override = default;

protected:
  void finalize_hdr(icmphdr *icmp, iphdr *ip) override;
};

#endif // XERXES_SMURF_H
