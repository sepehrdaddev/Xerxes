#ifndef RAW_SOCKET_H
#define RAW_SOCKET_H

#include <string>
#include <netdb.h>
#include <netinet/ip.h>

class Raw_Socket {
public:
	enum Protocol{TCP = IPPROTO_TCP, UDP = IPPROTO_UDP, ICMP = IPPROTO_ICMP};
  struct pshdr{
    u_int32_t src;
    u_int32_t dst;
    u_int8_t data;
    u_int8_t proto;
    u_int16_t len;
  };

  Raw_Socket();

  virtual void Create();
	static unsigned short checksum(unsigned short *buf, int len);
	virtual void Send(const char *buffer);
	virtual void Close();
	virtual void SetDst(struct sockaddr_in *d);
	virtual void SetIphdr(struct iphdr *ip);

  virtual ~Raw_Socket();
private:
	Protocol protocol = TCP;
	int sock = 0;
	struct sockaddr_in *dst = nullptr;
	struct iphdr *iph = nullptr;
	virtual void create();
};

#endif /* RAW_SOCKET_H */
