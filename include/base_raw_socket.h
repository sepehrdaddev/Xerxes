#ifndef XERXES_BASE_RAW_SOCKET_H
#define XERXES_BASE_RAW_SOCKET_H

#include "isocket.h"

#include <netdb.h>

class base_raw_socket : public ISocket {
public:
  base_raw_socket(std::string &rhost, std::string &rport, u_int8_t proto);
  virtual ~base_raw_socket() = default;
  ssize_t write(const char *str, size_t len) override;
  bool open() override;
  bool close() override;
  bool alive() override;

private:
  sockaddr_in dst{};
  std::string rhost{};
  std::string rport{};
  u_int8_t proto{};
  int fd{};
  int on{1};
};

#endif // XERXES_BASE_RAW_SOCKET_H