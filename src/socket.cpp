#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

#include "socket.h"

Socket::Socket(std::string host, std::string port, int sock_type) :
                rhost{std::move(host)}, rport{std::move(port)}, sock_type{sock_type}{

}

ssize_t Socket::Write(const char *str, size_t len) {
    return ::write(fd, str, len);
}

bool Socket::open() {
    struct addrinfo hints{}, *servinfo, *p;
    int rc;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    if((rc = getaddrinfo(rhost.c_str(), rport.c_str(), &hints, &servinfo))!= 0) {
        fprintf(stderr, "[-] getaddrinfo: %s\n", gai_strerror(rc));
        exit(0);
    }
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if(connect(fd, p->ai_addr, p->ai_addrlen)==-1) {
            ::close(fd);
            continue;
        }
        break;
    }
    if(p == nullptr) {
        if(servinfo)
            freeaddrinfo(servinfo);
        fprintf(stderr, "[-] No connection could be made\n");
        exit(0);
    }
    if(servinfo)
        freeaddrinfo(servinfo);
    fprintf(stderr, "[+] Connected -> %s:%s\n", rhost.c_str(), rport.c_str());
    return (fd > 0);
}

bool Socket::close() {
    return static_cast<bool>(::close(fd));
}

bool Socket::Alive() {
    return fd > 0;
}

bool Socket::Open() {
    return open();
}

bool Socket::Close() {
    return close();
}



