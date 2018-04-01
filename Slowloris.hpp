#ifndef XERXES_SLOWLORIS_H
#define XERXES_SLOWLORIS_H


#include "Configuration.hpp"
#include "Logger.hpp"
#include "Randomizer.hpp"
#include "Http_Flood.hpp"

class Slowloris: private Http_Flood{
    void attack(const int *id) override;
    void attack_ssl(const int *id) override;
};


#endif //XERXES_SLOWLORIS_H
