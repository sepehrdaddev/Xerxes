#ifndef XERXES_DOSER_H
#define XERXES_DOSER_H


#include <array>
#include "Configuration.h"
#include "Logger.h"

class Doser {
public:
    Doser(config *conf, Logger *logger);
    void run();

private:
    int make_socket(const char *host, const char *port);
    void read_socket(int socket);
    int write_socket(int socket, const char* string, int length);
    static void broke(int);
    std::string createStr();
    void attack(const int *id);
    std::string randomizeUserAgent();
    config *conf;
    Logger *logger;

};


#endif //XERXES_DOSER_H
