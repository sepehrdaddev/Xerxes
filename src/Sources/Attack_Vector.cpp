#include <unistd.h>

#include "../Headers/Attack_Vector.hpp"

Attack_Vector::Attack_Vector(std::shared_ptr<Config> config) : config{std::move(config)}{
}

void Attack_Vector::run() {
    for (int x = 0; x < config->THREADS; ++x) {
        if(fork()){
            attack();
        }
    }
    getc(stdin);
}

void Attack_Vector::pause() {
    if(config->delay > 0){
        usleep(static_cast<__useconds_t>(config->delay));
    }else{
        return;
    }
}
