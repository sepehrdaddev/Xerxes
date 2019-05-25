#ifndef XERXES_SYN_ACK_FLOOD_H
#define XERXES_SYN_ACK_FLOOD_H

#include "spoofed_tcp_flood.h"

class syn_ack_flood : public spoofed_tcp_flood {

public:
  syn_ack_flood();
  ~syn_ack_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_SYN_ACK_FLOOD_H