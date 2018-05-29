#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H

#include "Configuration.hpp"
#include "Logger.hpp"

class Engine {
public:
    explicit Engine(std::shared_ptr<Config> conf);
    void run();

private:
    std::shared_ptr<Config> conf;

};


#endif //XERXES_DOSER_H
