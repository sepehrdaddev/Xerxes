#ifndef XERXES_SPOOFED_TCP_FLOOD_H
#define XERXES_SPOOFED_TCP_FLOOD_H


#include "Spoofed_Flood.hpp"

class Spoofed_TCP_Flood : public Spoofed_Flood {
public:
    Spoofed_TCP_Flood(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;
    struct pseudo_header
    {
        u_int32_t source_address;
        u_int32_t dest_address;
        u_int8_t placeholder;
        u_int8_t protocol;
        u_int16_t tcp_length;
    };
};


#endif //XERXES_SPOOFED_TCP_FLOOD_H
