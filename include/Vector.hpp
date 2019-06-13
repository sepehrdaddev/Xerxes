#ifndef XERXES_VECTOR_HPP
#define XERXES_VECTOR_HPP

#include "Socket.hpp"
#include "config.hpp"
#include "stdafx.hpp"
#include "utils.hpp"

class IVector {
public:
  virtual void run() = 0;
};

template <typename T> class base_flood : public IVector {
protected:
  std::vector<T> sockets{};

  virtual int gen_hdr(T &socket) {}
  virtual void init_sockets() {}

public:
  void run() override;
};

template <typename T> class base_spoofed_flood : public IVector {
protected:
  std::vector<T> sockets{};

  virtual char *gen_hdr(T &socket) = 0;
  virtual void init_sockets() {}

public:
  void run() override;
};

#endif // XERXES_VECTOR_HPP