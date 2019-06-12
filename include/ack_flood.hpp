#ifndef XERXES_ACK_FLOOD_H
#define XERXES_ACK_FLOOD_H

#include "spoofed_tcp_flood.hpp"

class ack_flood : public spoofed_tcp_flood {

public:
  ack_flood();
  ~ack_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_ACK_FLOOD_H
