#ifndef XERXES_ACK_FLOOD_H
#define XERXES_ACK_FLOOD_H

#include "spoofed_tcp_flood.h"

class ack_flood : public spoofed_tcp_flood {

public:
  explicit ack_flood(std::shared_ptr<Config> config);
  ~ack_flood() override = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_ACK_FLOOD_H
