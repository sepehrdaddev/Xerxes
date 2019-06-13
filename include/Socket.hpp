#ifndef XERXES_SOCKET_HPP
#define XERXES_SOCKET_HPP

#include "stdafx.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <tls.h>

class ISocket {
public:
  virtual ssize_t write(const char *str, size_t len) = 0;
  virtual bool open() = 0;
  virtual bool close() = 0;
  virtual bool alive() = 0;
};

class base_socket : public ISocket {
protected:
  addrinfo hints{}, *servinfo{};
  std::string rhost{}, rport{};
  int fd{};

public:
  base_socket(std::string &rhost, std::string &rport, __socket_type socktype)
      : rhost{std::move(rhost)}, rport{std::move(rport)} {

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = socktype;
  }

  ssize_t write(const char *str, size_t len) override {
    return ::write(fd, str, len);
  }

  bool open() override {
    addrinfo *p{};

    if (int rc{getaddrinfo(rhost.c_str(), rport.c_str(), &hints, &servinfo)};
        rc != 0) {
      std::clog << "getaddrinfo: " << gai_strerror(rc) << '\n';
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
      std::clog << "No connection could be made\n";
      exit(EXIT_FAILURE);
    }
    if (servinfo)
      freeaddrinfo(servinfo);
    std::clog << "Connected -> " << rhost << ':' << rport << '\n';
    return (fd > 0);
  }

  bool close() override { return static_cast<bool>(::close(fd)); }

  bool alive() override { return (fd > 0); }

  virtual ~base_socket() = default;
};

class base_raw_socket : public ISocket {
protected:
  sockaddr_in dst{};
  std::string rhost{}, rport{};
  u_int8_t proto{};
  int fd{}, on{1}, hdr_len{8192};

public:
  base_raw_socket(std::string &rhost, std::string &rport, u_int8_t proto)
      : rhost{std::move(rhost)}, rport{std::move(rport)}, proto{proto} {}

  ssize_t write(const char *str, size_t len) override {
    return static_cast<int>(sendto(fd, str, len, 0,
                                   reinterpret_cast<sockaddr *>(&dst),
                                   sizeof(sockaddr_in)));
  }

  bool open() override {
    if ((fd = socket(AF_INET, SOCK_RAW, proto)) == -1) {
      std::clog << "socket error\n";
      exit(EXIT_FAILURE);
    }

    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1) {
      std::clog << "setsockopt error: " << strerror(errno) << '\n';
      exit(EXIT_FAILURE);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST,
                   reinterpret_cast<const char *>(&on), sizeof(on)) == -1) {
      std::clog << "setsockopt error: " << strerror(errno) << '\n';
      exit(EXIT_FAILURE);
    }

    if (hostent * hp{gethostbyname(rhost.c_str())}; hp == nullptr) {
      dst.sin_addr.s_addr = inet_addr(rhost.c_str());
      if (dst.sin_addr.s_addr == INADDR_NONE) {
        std::clog << "can't resolve the host\n";
        exit(EXIT_FAILURE);
      }
    } else {
      bcopy(hp->h_addr_list[0], &dst.sin_addr.s_addr,
            static_cast<size_t>(hp->h_length));
    }

    dst.sin_family = AF_UNSPEC;
    bzero(dst.sin_zero, sizeof(dst.sin_zero));
    std::clog << "Connected -> " << rhost << ':' << rport << '\n';
    return (fd > 0);
  }

  bool close() override { return static_cast<bool>(::close(fd)); }

  bool alive() override { return (fd > 0); }

  virtual ~base_raw_socket() = default;
};

class tcp : public base_socket {
public:
  tcp(std::string &rhost, std::string &rport)
      : base_socket(rhost, rport, SOCK_STREAM) {}

  virtual ~tcp() = default;
};

class udp : public base_socket {
public:
  udp(std::string &rhost, std::string &rport)
      : base_socket(rhost, rport, SOCK_DGRAM) {}

  virtual ~udp() = default;
};

class ssl_tcp : public tcp {
  struct tls *tls = nullptr;
  struct tls_config *tls_config = nullptr;

public:
  ssl_tcp(std::string &rhost, std::string &rport) : tcp(rhost, rport) {}

  ssize_t write(const char *str, size_t len) override {
    return tls_write(tls, str, len);
  }

  bool open() override {
    int rc;
    tcp::open();
    tls_init();
    tls = tls_client();
    tls_config = tls_config_new();
    tls_config_insecure_noverifycert(tls_config);
    tls_config_insecure_noverifyname(tls_config);

    tls_configure(tls, tls_config);
    if ((rc = tls_connect_socket(tls, fd, rhost.c_str())) < 0) {
      std::clog << "tls_connect error: " << tls_error(tls) << '\n';
      exit(EXIT_FAILURE);
    }
    return (rc > 0);
  }

  bool close() override {
    tls_close(tls);
    if (tls)
      tls_free(tls);
    if (tls_config)
      tls_config_free(tls_config);
    return tcp::close();
  }

  bool alive() override {
    return tcp::alive() && (tls != nullptr) && (tls_config != nullptr);
  }

  virtual ~ssl_tcp() = default;
};

class raw_tcp : public base_raw_socket {
public:
  raw_tcp(std::string &rhost, std::string &rport)
      : base_raw_socket(rhost, rport, IPPROTO_TCP) {}

  virtual ~raw_tcp() = default;
};

class raw_udp : public base_raw_socket {
public:
  raw_udp(std::string &rhost, std::string &rport)
      : base_raw_socket(rhost, rport, IPPROTO_UDP) {}

  virtual ~raw_udp() = default;
};

class icmp : public base_raw_socket {
public:
  icmp(std::string &rhost, std::string &rport)
      : base_raw_socket(rhost, rport, IPPROTO_ICMP) {
    hdr_len = 400;
  }

  virtual ~icmp() = default;
};

#endif // XERXES_SOCKET_HPP