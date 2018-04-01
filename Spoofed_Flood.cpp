#include <unistd.h>
#include "Spoofed_Flood.hpp"

Spoofed_Flood::Spoofed_Flood(const config *conf, Logger *logger) : conf{conf}, logger{logger}{

}

void Spoofed_Flood::run() {
    for (int x = 0; x < conf->THREADS; x++) {
        if(fork()){
            attack(&x);
        }
    }
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

void Spoofed_Flood::attack(const int *id) {

}
