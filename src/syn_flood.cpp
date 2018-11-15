#include "syn_flood.h"

syn_flood::syn_flood(std::shared_ptr<Config> config) : spoofed_tcp_flood(std::move(config)) {

}
