#include "null_udp.h"

null_udp::null_udp(std::shared_ptr<Config> config)
    : udp_flood(std::move(config)) {}

int null_udp::gen_hdr(std::string &string) {
  string = "\0";
  return 1;
}
