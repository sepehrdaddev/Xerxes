#ifndef XERXES_UDP_H
#define XERXES_UDP_H

#include "base_socket.h"
#include "stdafx.h"

#include <sys/socket.h>

class udp : public base_socket {
public:
  udp(std::string &host, std::string &port);
  ~udp() = default;
};

#endif // XERXES_UDP_H