#include "udp_flood.hpp"

udp_flood::udp_flood() : base_flood(SOCK_DGRAM) {}

null_udp::null_udp() : udp_flood() {}

int null_udp::gen_hdr(std::string &string) {
  string = "\0";
  return 1;
}
