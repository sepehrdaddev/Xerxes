#ifndef XERXES_LAND_H
#define XERXES_LAND_H

#include "spoofed_tcp_flood.hpp"

class land : public spoofed_tcp_flood {

public:
  land();
  ~land() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_LAND_H
