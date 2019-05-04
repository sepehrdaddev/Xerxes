#ifndef XERXES_SYN_FLOOD_H
#define XERXES_SYN_FLOOD_H

#include "spoofed_tcp_flood.h"

class syn_flood : public spoofed_tcp_flood {

public:
  syn_flood();
  ~syn_flood() = default;
};

#endif // XERXES_SYN_FLOOD_H
