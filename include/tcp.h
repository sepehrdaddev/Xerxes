#ifndef XERXES_TCP_H
#define XERXES_TCP_H

#include "base_socket.h"
#include "stdafx.h"

#include <sys/socket.h>

class tcp : public base_socket {
public:
  tcp(std::string &host, std::string &port);
  ~tcp() = default;
};

#endif // XERXES_TCP_H