#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "rsocket.h"

Rsocket::Rsocket(std::string host, std::string port, int proto) :
                    rhost{std::move(host)}, rport{std::move(port)}, proto{proto} {
}

bool Rsocket::open() {
    int on = 1;
    if((fd = socket(AF_INET, SOCK_RAW, proto)) == -1){
        fputs("[-] socket error\n", stderr);
        exit(EXIT_FAILURE);
    }

    if(setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1){
        fputs("[-] setsockopt error\n", stderr);
        exit(EXIT_FAILURE);
    }

    if(setsockopt(fd, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&on), sizeof(on)) == -1){
        fputs("[-] setsockopt error\n", stderr);
        exit(EXIT_FAILURE);
    }

    hostent *hp{};
    if((hp = gethostbyname(rhost.c_str())) == nullptr){
        if((dst.sin_addr.s_addr = inet_addr(rhost.c_str())) == -1){
            fputs("[-] can't resolve the host\n", stderr);
            exit(EXIT_FAILURE);
        }
    }else{
        bcopy(hp->h_addr_list[0], &dst.sin_addr.s_addr, static_cast<size_t>(hp->h_length));
    }

    dst.sin_family = AF_UNSPEC;

    return (fd > 0);
}

bool Rsocket::close() {
    return static_cast<bool>(::close(fd));
}

bool Rsocket::Open() {
    return open();
}

bool Rsocket::Close() {
    return close();
}

bool Rsocket::Alive() {
    return (fd > 0);
}

ssize_t Rsocket::Write(const char *str, size_t len) {
    return static_cast<int>(sendto(fd, str, len, 0, reinterpret_cast<sockaddr*>(&dst), sizeof(struct sockaddr_in)));
}

unsigned short Rsocket::csum(unsigned short *buf, int len) {
    unsigned long sum;
    for(sum=0; len>0; len--){
        sum += *buf++;
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
