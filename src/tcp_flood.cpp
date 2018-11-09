#include "tcp_flood.h"

#include "utils.h"

void tcp_flood::run() {

}

void tcp_flood::gen_hdr(std::string &string) {
    utils::randomizer::randomstr(string);
}

tcp_flood::tcp_flood(std::shared_ptr<Config> config) : config{std::move(config)}{

}
