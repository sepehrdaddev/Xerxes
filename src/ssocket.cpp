#include "ssocket.h"

#include <iostream>
#include <sys/socket.h>

#include <spdlog/spdlog.h>

Ssocket::Ssocket(std::string host, std::string port)
    : Socket(std::move(host), std::move(port), SOCK_STREAM) {}

ssize_t Ssocket::Write(const char *str, size_t len) {
  return tls_write(tls, str, len);
}

bool Ssocket::Open() {
  int rc;
  Socket::open();
  tls_init();
  tls = tls_client();
  tls_config = tls_config_new();
  tls_config_insecure_noverifycert(tls_config);
  tls_config_insecure_noverifyname(tls_config);

  tls_configure(tls, tls_config);
  if ((rc = tls_connect_socket(tls, fd, rhost.c_str())) < 0) {
    spdlog::get("logger")->error(std::string{"tls_connect error: "} +
                                 tls_error(tls));
    exit(EXIT_FAILURE);
  }
  return (rc > 0);
}

bool Ssocket::Close() {
  tls_close(tls);
  if (tls)
    tls_free(tls);
  if (tls_config)
    tls_config_free(tls_config);
  return Socket::close();
}

bool Ssocket::Alive() {
  return Socket::Alive() && (tls != nullptr) && (tls_config != nullptr);
}
