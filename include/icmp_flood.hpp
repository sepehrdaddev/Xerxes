#ifndef XERXES_ICMP_FLOOD_H
#define XERXES_ICMP_FLOOD_H

#include "base_spoofed_flood.hpp"

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

class icmp_flood : public base_spoofed_flood {

public:
  icmp_flood();
  ~icmp_flood() = default;

protected:
  char *gen_hdr(sockaddr_in *dst, int len) override;
  virtual void init_hdr(icmphdr *icmp, iphdr *ip);
  virtual void finalize_hdr(icmphdr *icmp, iphdr *ip);
};

class blacknurse : public icmp_flood {

public:
  blacknurse();
  ~blacknurse() = default;

protected:
  void finalize_hdr(icmphdr *icmp, iphdr *ip) override;
};

class smurf : public icmp_flood {

public:
  smurf();
  ~smurf() = default;

protected:
  void finalize_hdr(icmphdr *icmp, iphdr *ip) override;
};

#endif // XERXES_ICMP_FLOOD_H
