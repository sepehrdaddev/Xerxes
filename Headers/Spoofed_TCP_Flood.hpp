#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H

#include "Spoofed_Flood.hpp"

class Spoofed_TCP_Flood : public Spoofed_Flood {
public:
    Spoofed_TCP_Flood(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;
};


#endif //XERXES_SPOOFED_TCP_FLOOD_H
