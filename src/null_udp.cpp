#include "null_udp.hpp"

null_udp::null_udp() : udp_flood() {}

int null_udp::gen_hdr(std::string &string) {
  string = "\0";
  return 1;
}
