#ifndef XERXES_FIN_FLOOD_H
#define XERXES_FIN_FLOOD_H

#include "spoofed_tcp_flood.hpp"

class fin_flood : public spoofed_tcp_flood {

public:
  fin_flood();
  ~fin_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_FIN_FLOOD_H
