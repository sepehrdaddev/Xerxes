#ifndef XERXES_SOCKET_H
#define XERXES_SOCKET_H


#include <cstdio>
#include <string>

class Socket {

public:
    Socket(std::string host, std::string port, int sock_type);
    virtual ~Socket() = default;
    virtual ssize_t Write(const char *str, size_t len);
    virtual bool Open();
    virtual bool Close();
    virtual bool Alive();

protected:
    bool open();
    bool close();
    std::string rhost;
    std::string rport;
    int fd;
    int sock_type;
};


#endif //XERXES_SOCKET_H
