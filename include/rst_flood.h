#ifndef XERXES_RST_FLOOD_H
#define XERXES_RST_FLOOD_H

#include "spoofed_tcp_flood.h"

class rst_flood : public spoofed_tcp_flood {

public:
  explicit rst_flood(std::shared_ptr<Config> config);
  ~rst_flood() override = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_RST_FLOOD_H