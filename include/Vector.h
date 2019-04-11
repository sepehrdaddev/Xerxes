#ifndef XERXES_VECTOR_H
#define XERXES_VECTOR_H

#include <memory>

#include "config.h"

class Vector {
public:
  explicit Vector(std::shared_ptr<Config> config);
  virtual ~Vector() = default;
  virtual void run() = 0;

protected:
  std::shared_ptr<Config> config;
};

#endif // XERXES_VECTOR_H
