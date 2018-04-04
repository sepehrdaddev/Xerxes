
#include "Spoofed_Flood.hpp"

Spoofed_Flood::Spoofed_Flood(const config *conf, Logger *logger) : Attack_Vector(conf, logger){

}

unsigned short Spoofed_Flood::csum(unsigned short *buf, int len) {
    unsigned long sum;
    for(sum=0; len>0; len--){
        sum += *buf++;
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
