#include "base_socket.h"

base_socket::base_socket(std::string &rhost, std::string &rport,
                         __socket_type socktype)
    : rhost{rhost}, rport{rport} {

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = socktype;
}

ssize_t base_socket::write(const char *str, size_t len) {
  return ::write(fd, str, len);
}

bool base_socket::open() {
  addrinfo *p{};
  int rc{getaddrinfo(rhost.c_str(), rport.c_str(), &hints, &servinfo)};
  if (rc != 0) {
    spdlog::get("logger")->error("getaddrinfo: {0}", gai_strerror(rc));
    exit(EXIT_FAILURE);
  }
  for (p = servinfo; p != nullptr; p = p->ai_next) {
    if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      continue;
    if (connect(fd, p->ai_addr, p->ai_addrlen) == -1) {
      ::close(fd);
      continue;
    }
    break;
  }
  if (p == nullptr) {
    if (servinfo)
      freeaddrinfo(servinfo);
    spdlog::get("logger")->error("No connection could be made");
    exit(EXIT_FAILURE);
  }
  if (servinfo)
    freeaddrinfo(servinfo);
  spdlog::get("logger")->info("Connected -> {0}:{1}", rhost, rport);
  return (fd > 0);
}

bool base_socket::close() { return static_cast<bool>(::close(fd)); }

bool base_socket::alive() { return (fd > 0); }