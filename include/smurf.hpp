#ifndef XERXES_SMURF_H
#define XERXES_SMURF_H

#include "icmp_flood.hpp"

class smurf : public icmp_flood {

public:
  smurf();
  ~smurf() = default;

protected:
  void finalize_hdr(icmphdr *icmp, iphdr *ip) override;
};

#endif // XERXES_SMURF_H
