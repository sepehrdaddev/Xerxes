#include "tcp.h"

tcp::tcp(std::string &host, std::string &port)
    : base_socket(host, port, SOCK_STREAM) {}