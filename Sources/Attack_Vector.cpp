#include <unistd.h>

#include "../Headers/Attack_Vector.hpp"

Attack_Vector::Attack_Vector(const config *conf, Logger *logger) : conf{conf}, logger{logger}{

}

void Attack_Vector::run() {
    for (int x = 0; x < conf->THREADS; x++) {
        if(fork()){
            attack(&x);
        }
    }
    getc(stdin);
}

void Attack_Vector::pause() {
    if(conf->delay > 0){
        usleep(static_cast<__useconds_t>(conf->delay));
    }else{
        return;
    }
}