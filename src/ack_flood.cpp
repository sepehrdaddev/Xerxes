#include "ack_flood.h"

ack_flood::ack_flood(std::shared_ptr<Config> config)
    : spoofed_tcp_flood(std::move(config)) {}

void ack_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_ACK;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
