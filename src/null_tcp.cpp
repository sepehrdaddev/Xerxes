#include "null_tcp.h"

null_tcp::null_tcp(std::shared_ptr<Config> config)
    : tcp_flood(std::move(config)) {}

int null_tcp::gen_hdr(std::string &string) {
  string = "\0";
  return 1;
}
