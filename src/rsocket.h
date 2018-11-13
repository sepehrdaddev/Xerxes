#ifndef XERXES_RSOCKET_H
#define XERXES_RSOCKET_H

#include <string>
#include <vector>
#include <netdb.h>

class Rsocket {

public:
    Rsocket(std::string host, std::string port, int proto);
    virtual ~Rsocket() = default;
    virtual ssize_t Write(const char *str, size_t len);
    virtual bool Open();
    virtual bool Close();
    virtual bool Alive();
    unsigned short csum(unsigned short *buf, int len);

protected:
    bool open();
    bool close();
    std::string rhost;
    std::string rport;
    int proto;
    int fd;
    sockaddr_in dst;
};


#endif //XERXES_RSOCKET_H
