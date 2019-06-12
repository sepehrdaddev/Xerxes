#ifndef XERXES_VECTOR_HPP
#define XERXES_VECTOR_HPP

#include "Socket.hpp"
#include "stdafx.hpp"

class IVector {
public:
  virtual void run() = 0;
};

class base_flood : public IVector {
protected:
  virtual int gen_hdr(std::string &string);
  virtual void init_sockets(std::vector<std::unique_ptr<base_socket>> &sockets);

public:
  explicit base_flood(int sock_type);
  ~base_flood() = default;
  void run() override;
};

class base_spoofed_flood : public IVector {
protected:
  virtual char *gen_hdr(sockaddr_in *dst, int len) = 0;
  virtual void
  init_sockets(std::vector<std::unique_ptr<base_raw_socket>> &sockets);
  int proto{};
  int hdr_len{};

public:
  explicit base_spoofed_flood(int protocol);
  ~base_spoofed_flood() = default;
  void run() override;
};

#endif // XERXES_VECTOR_HPP