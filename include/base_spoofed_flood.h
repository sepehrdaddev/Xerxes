#ifndef XERXES_BASE_SPOOFED_FLOOD_H
#define XERXES_BASE_SPOOFED_FLOOD_H

#include "rsocket.h"
#include "vector.h"

class base_spoofed_flood : public IVector {
public:
  explicit base_spoofed_flood(int protocol);
  ~base_spoofed_flood() = default;
  void run() override;

protected:
  virtual char *gen_hdr(sockaddr_in *dst, int len) = 0;
  virtual void init_sockets(std::vector<std::unique_ptr<Rsocket>> &sockets);
  int proto{};
  int hdr_len{};
};

#endif // XERXES_BASE_SPOOFED_FLOOD_H
