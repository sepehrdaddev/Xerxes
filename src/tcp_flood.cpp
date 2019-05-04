#include "tcp_flood.h"

#include <arpa/inet.h>

tcp_flood::tcp_flood() : base_flood(SOCK_STREAM) {}
