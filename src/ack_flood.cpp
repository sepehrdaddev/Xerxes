#include "ack_flood.hpp"

ack_flood::ack_flood() : spoofed_tcp_flood() {}

void ack_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_ACK;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
