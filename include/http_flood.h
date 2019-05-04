#ifndef XERXES_HTTP_FLOOD_H
#define XERXES_HTTP_FLOOD_H

#include "tcp_flood.h"

class http_flood : public tcp_flood {
public:
  http_flood();
  ~http_flood() = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_HTTP_FLOOD_H
