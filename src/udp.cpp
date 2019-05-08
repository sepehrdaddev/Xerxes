#include "udp.h"

udp::udp(std::string &host, std::string &port)
    : base_socket(host, port, SOCK_DGRAM) {}