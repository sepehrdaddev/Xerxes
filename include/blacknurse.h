#ifndef XERXES_BLACKNURSE_H
#define XERXES_BLACKNURSE_H

#include "icmp_flood.h"

class blacknurse : public icmp_flood {

public:
  blacknurse();
  ~blacknurse() = default;

protected:
  void finalize_hdr(icmphdr *icmp, iphdr *ip) override;
};

#endif // XERXES_BLACKNURSE_H
