#ifndef XERXES_XMAS_FLOOD_H
#define XERXES_XMAS_FLOOD_H

#include "spoofed_tcp_flood.hpp"

class xmas_flood : public spoofed_tcp_flood {

public:
  xmas_flood();
  ~xmas_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_XMAS_FLOOD_H