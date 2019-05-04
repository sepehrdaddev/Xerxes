#ifndef XERXES_ENGINE_H
#define XERXES_ENGINE_H

#include "config.h"

class engine {

public:
  engine();

  engine(const engine &) = default;
  engine &operator=(const engine &) = default;

  engine(engine &&) = default;
  engine &operator=(engine &&) = default;

  ~engine() = default;
};

#endif // XERXES_ENGINE_H
