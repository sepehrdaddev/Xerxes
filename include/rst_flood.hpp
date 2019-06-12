#ifndef XERXES_RST_FLOOD_H
#define XERXES_RST_FLOOD_H

#include "spoofed_tcp_flood.hpp"

class rst_flood : public spoofed_tcp_flood {

public:
  rst_flood();
  ~rst_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_RST_FLOOD_H