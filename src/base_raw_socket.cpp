#include "base_raw_socket.h"

#include <arpa/inet.h>
#include <unistd.h>

base_raw_socket::base_raw_socket(std::string &rhost, std::string &rport,
                                 u_int8_t proto)
    : rhost{std::move(rhost)}, rport{std::move(rport)}, proto{proto} {}

ssize_t base_raw_socket::write(const char *str, size_t len) {
  return static_cast<int>(sendto(fd, str, len, 0,
                                 reinterpret_cast<sockaddr *>(&dst),
                                 sizeof(sockaddr_in)));
}

bool base_raw_socket::open() {
  if ((fd = socket(AF_INET, SOCK_RAW, proto)) == -1) {
    spdlog::get("logger")->error("socket error");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1) {
    spdlog::get("logger")->error("setsockopt error: {0}", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST,
                 reinterpret_cast<const char *>(&on), sizeof(on)) == -1) {
    spdlog::get("logger")->error("setsockopt error: {0}", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (hostent * hp{gethostbyname(rhost.c_str())}; hp == nullptr) {
    dst.sin_addr.s_addr = inet_addr(rhost.c_str());
    if (dst.sin_addr.s_addr == INADDR_NONE) {
      spdlog::get("logger")->error("can't resolve the host");
      exit(EXIT_FAILURE);
    }
  } else {
    bcopy(hp->h_addr_list[0], &dst.sin_addr.s_addr,
          static_cast<size_t>(hp->h_length));
  }

  dst.sin_family = AF_UNSPEC;
  bzero(dst.sin_zero, sizeof(dst.sin_zero));
  spdlog::get("logger")->info("Connected -> {0}:{1}", rhost, rport);
  return (fd > 0);
}
bool base_raw_socket::close() { return static_cast<bool>(::close(fd)); }
bool base_raw_socket::alive() { return (fd > 0); }