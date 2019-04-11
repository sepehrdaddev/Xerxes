#ifndef XERXES_NULL_UDP_H
#define XERXES_NULL_UDP_H

#include "udp_flood.h"

class null_udp : public udp_flood {
public:
  explicit null_udp(std::shared_ptr<Config> config);

  ~null_udp() override = default;

protected:
  int gen_hdr(std::string &string) override;
};

#endif // XERXES_NULL_UDP_H
