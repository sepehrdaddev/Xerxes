#include "syn_ack.h"

syn_ack_flood::syn_ack_flood() : spoofed_tcp_flood() {}

void syn_ack_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_SYN | TH_ACK;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
