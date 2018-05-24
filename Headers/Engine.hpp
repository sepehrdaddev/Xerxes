#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H

#include "Configuration.hpp"
#include "Logger.hpp"

class Engine {
public:
    Engine(Config *conf, Logger *logger);
    void run();

private:
    Config *conf;
    Logger *logger;

};


#endif //XERXES_DOSER_H
