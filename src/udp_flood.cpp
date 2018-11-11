#include "udp_flood.h"

udp_flood::udp_flood(std::shared_ptr<Config> config) : base_flood(std::move(config), SOCK_DGRAM) {

}
