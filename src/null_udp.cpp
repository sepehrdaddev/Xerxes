#include "null_udp.h"

null_udp::null_udp() : udp_flood() {}

int null_udp::gen_hdr(std::string &string) {
  string = "\0";
  return 1;
}
