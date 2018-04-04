#ifndef XERXES_ICMP_FLOOD_H
#define XERXES_ICMP_FLOOD_H

#include "Spoofed_Flood.hpp"

class ICMP_Flood : public Spoofed_Flood {
public:
    ICMP_Flood(const config *conf, Logger *logger);
private:
    void attack(const int *id) override;

};


#endif //XERXES_ICMP_FLOOD_H
