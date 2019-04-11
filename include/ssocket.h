#ifndef XERXES_SSOCKET_H
#define XERXES_SSOCKET_H

#include "socket.h"
#include <tls.h>

class Ssocket : public Socket {
public:
  Ssocket(std::string host, std::string port);
  ~Ssocket() override = default;

  ssize_t Write(const char *str, size_t len) override;
  bool Open() override;
  bool Close() override;
  bool Alive() override;

protected:
  struct tls *tls = nullptr;
  struct tls_config *tls_config = nullptr;
};

#endif // XERXES_SSOCKET_H
