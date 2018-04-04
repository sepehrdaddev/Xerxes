#ifndef XERXES_NULL_FLOOD_H
#define XERXES_NULL_FLOOD_H


#include "Configuration.hpp"
#include "Logger.hpp"
#include "Http_Flood.hpp"

class Null_Flood : public Http_Flood {
public:
    Null_Flood(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;
    void attack_ssl(const int *id) override;
    int make_socket(const char *host, const char *port) override;

};


#endif //XERXES_NULL_FLOOD_H
