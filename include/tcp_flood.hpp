#ifndef XERXES_TCP_FLOOD_H
#define XERXES_TCP_FLOOD_H

#include "base_flood.hpp"
#include "config.hpp"
#include "stdafx.hpp"

class tcp_flood : public base_flood {

public:
  tcp_flood();
  ~tcp_flood() = default;
};

class null_tcp : public tcp_flood {
public:
  null_tcp();

  ~null_tcp() = default;

protected:
  int gen_hdr(std::string &string) override;
};

class http_flood : public tcp_flood {
public:
  http_flood();
  ~http_flood() = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_TCP_FLOOD_H
