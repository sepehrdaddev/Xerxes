#ifndef XERXES_CONFIG_H
#define XERXES_CONFIG_H

#include "stdafx.hpp"

enum __Vector__ {
  NULL_TCP = 0,
  NULL_UDP,
  TCP_FLOOD,
  UDP_FLOOD,
  HTTP_FLOOD,
  ICMP_FLOOD,
  SYN_FLOOD,
  ACK_FLOOD,
  FIN_FLOOD,
  SPOOFED_UDP_FLOOD,
  TEARDROP,
  BLACKNURSE,
  LAND,
  SMURF,
  ACK_PSH_FLOOD,
  RST_FLOOD,
  SYN_ACK,
  XMAS_FLOOD
};

class Config {
public:
  static Config &get() {
    static Config config{};
    return config;
  }
  std::string rhost{};
  std::string rport{};
  std::string bcast{};
  unsigned int vec{};
  unsigned int trds{};
  unsigned int conn{};
  unsigned int dly{};
  bool tls{};
  bool rand_lhost{};
  bool rand_lport{};
  bool daemonize{};

private:
  Config() = default;

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  Config(Config &&) = delete;
  Config &operator=(Config &&) = delete;

  ~Config() = default;
};

#endif // XERXES_CONFIG_H
