#ifndef XERXES_ACK_PSH_FLOOD_H
#define XERXES_ACK_PSH_FLOOD_H

#include "spoofed_tcp_flood.h"

class ack_psh_flood : public spoofed_tcp_flood {

public:
  ack_psh_flood();
  ~ack_psh_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_ACK_PSH_FLOOD_H