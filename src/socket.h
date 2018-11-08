#ifndef XERXES_SOCKET_H
#define XERXES_SOCKET_H


#include <cstdio>

class Socket {

public:
    Socket(const char *host, const char *port, int sock_type);
    virtual ~Socket() = default;
    virtual ssize_t Write(const char *str, size_t len);
    virtual bool Open();
    virtual bool Close();
    virtual bool Alive();

protected:
    bool open();
    bool close();
    const char *rhost;
    const char *rport;
    int fd;
    int sock_type;
};


#endif //XERXES_SOCKET_H
