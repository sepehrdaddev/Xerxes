#include "tcp_flood.hpp"

#include <arpa/inet.h>

tcp_flood::tcp_flood() : base_flood(SOCK_STREAM) {}
