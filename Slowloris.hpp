#ifndef XERXES_SLOWLORIS_H
#define XERXES_SLOWLORIS_H


#include "Configuration.hpp"
#include "Logger.hpp"
#include "Randomizer.hpp"
#include "Http_Flood.hpp"

class Slowloris : public Http_Flood {
public:
    Slowloris(const config *conf, Logger *logger);
private:
    void attack(const int *id) override;
    void attack_ssl(const int *id) override;
};


#endif //XERXES_SLOWLORIS_H
