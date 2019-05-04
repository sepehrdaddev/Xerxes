#ifndef XERXES_TCP_FLOOD_H
#define XERXES_TCP_FLOOD_H

#include "base_flood.h"
#include "config.h"
#include "stdafx.h"

class tcp_flood : public base_flood {

public:
  tcp_flood();
  ~tcp_flood() = default;
};

#endif // XERXES_TCP_FLOOD_H
