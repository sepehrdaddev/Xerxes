#ifndef XERXES_SSL_FLOOD_H
#define XERXES_SSL_FLOOD_H


#include "Attack_Vector.hpp"

class Ssl_Flood : public Attack_Vector {
public:
    Ssl_Flood(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;
};


#endif //XERXES_SSL_FLOOD_H
