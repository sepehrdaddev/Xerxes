#ifndef XERXES_NULL_TCP_H
#define XERXES_NULL_TCP_H

#include "tcp_flood.hpp"

class null_tcp : public tcp_flood {
public:
  null_tcp();

  ~null_tcp() = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_NULL_TCP_H
