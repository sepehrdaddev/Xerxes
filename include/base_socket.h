#ifndef XERXES_BASE_SOCKET_H
#define XERXES_BASE_SOCKET_H

#include "isocket.h"
#include "stdafx.h"

#include <netdb.h>

class base_socket : public ISocket {
public:
  base_socket(std::string &host, std::string &port, __socket_type socktype);
  virtual ~base_socket() = default;
  ssize_t write(const char *str, size_t len) override;
  bool open() override;
  bool close() override;
  bool alive() override;

private:
  addrinfo hints{};
  addrinfo *servinfo{};
  std::string rhost{};
  std::string rport{};
  int fd{};
};

#endif // XERXES_BASE_SOCKET_H