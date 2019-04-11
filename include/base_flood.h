#ifndef XERXES_BASE_FLOOD_H
#define XERXES_BASE_FLOOD_H

#include "Vector.h"
#include "socket.h"
#include <vector>

class base_flood : public Vector {

public:
  base_flood(std::shared_ptr<Config> config, int sock_type);
  ~base_flood() override = default;
  void run() override;

protected:
  virtual int gen_hdr(std::string &string);
  virtual void init_sockets(std::vector<std::unique_ptr<Socket>> &sockets);
  int sock_type{};
};

#endif // XERXES_BASE_FLOOD_H
