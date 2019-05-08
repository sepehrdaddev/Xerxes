#ifndef XERXES_BASE_FLOOD_H
#define XERXES_BASE_FLOOD_H

#include "ivector.h"
#include "socket.h"
#include "stdafx.h"

class base_flood : public IVector {

public:
  explicit base_flood(int sock_type);
  ~base_flood() = default;
  void run() override;

protected:
  virtual int gen_hdr(std::string &string);
  virtual void init_sockets(std::vector<std::unique_ptr<Socket>> &sockets);
  int sock_type{};
};

#endif // XERXES_BASE_FLOOD_H
