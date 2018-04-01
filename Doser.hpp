#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H


#include <array>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "Configuration.hpp"
#include "Logger.hpp"

class Doser {
public:
    Doser(config *conf, Logger *logger);
    void run();

private:
    config *conf;
    Logger *logger;

};


#endif //XERXES_DOSER_H
