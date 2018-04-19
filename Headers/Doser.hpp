#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H

#include "Configuration.hpp"
#include "Logger.hpp"

class Doser {
public:
    Doser(config *conf, Logger *logger);
    void run();

private:
    config *conf;
    Logger *logger;
    void show_info();

};


#endif //XERXES_DOSER_H
