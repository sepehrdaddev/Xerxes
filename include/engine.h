#ifndef XERXES_ENGINE_H
#define XERXES_ENGINE_H

#include "config.h"

class Engine {

public:
  void start();
  static Engine &get();

private:
  Engine() = default;

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  Engine(Engine &&) = delete;
  Engine &operator=(Engine &&) = delete;

  ~Engine() = default;
};

#endif // XERXES_ENGINE_H
