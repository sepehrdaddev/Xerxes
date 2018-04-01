#ifndef XERXES_SPOOFED_UDP_FLOOD_H
#define XERXES_SPOOFED_UDP_FLOOD_H


#include "Spoofed_Flood.hpp"

class Spoofed_UDP_Flood: public Spoofed_Flood {
public:
    Spoofed_UDP_Flood(const config *conf, Logger *logger);
private:
    void attack(const int *id) override;
};


#endif //XERXES_SPOOFED_UDP_FLOOD_H
