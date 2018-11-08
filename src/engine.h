#ifndef XERXES_ENGINE_H
#define XERXES_ENGINE_H

#include <memory>

#include "config.h"

class engine {
    explicit  engine(std::shared_ptr<Config> config);
    virtual ~engine() = default;
};


#endif //XERXES_ENGINE_H
