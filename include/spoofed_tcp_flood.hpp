#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H

#include "base_spoofed_flood.hpp"

#include <netinet/ip.h>
#include <netinet/tcp.h>

class spoofed_tcp_flood : public base_spoofed_flood {

public:
  spoofed_tcp_flood();
  ~spoofed_tcp_flood() = default;

protected:
  char *gen_hdr(sockaddr_in *dst, int len) override;
  virtual void init_hdr(tcphdr *tcp, iphdr *ip);
  virtual void finalize_hdr(tcphdr *tcp, iphdr *ip);

  struct pshdr {
    u_int32_t saddr = 0;
    u_int32_t daddr = 0;
    u_int8_t zero = 0;
    u_int8_t proto = IPPROTO_TCP;
    u_int16_t len = 0;
    tcphdr tcp;
  };
};

#endif // XERXES_SPOOFED_TCP_FLOOD_H
