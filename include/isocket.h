#ifndef XERXES_ISOCKET_H
#define XERXES_ISOCKET_H

#include "stdafx.h"

class ISocket {
public:
  virtual ssize_t write(const char *str, size_t len) = 0;
  virtual bool open() = 0;
  virtual bool close() = 0;
  virtual bool alive() = 0;
};

#endif // XERXES_ISOCKET_H