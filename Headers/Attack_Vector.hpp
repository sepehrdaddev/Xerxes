#ifndef XERXES_ATTACK_VECTOR_H
#define XERXES_ATTACK_VECTOR_H

#include "Configuration.hpp"
#include "Logger.hpp"

class Attack_Vector {
    friend class Spoofed_Flood;
    friend class Http_Flood;
    friend class Beast;

public:
    Attack_Vector() = default;
    Attack_Vector(const config *conf, Logger *logger);
    virtual void run();

protected:
    const config *conf;
    Logger *logger;
    virtual void pause();

private:
    virtual void attack(const int *id) = 0;
};


#endif //XERXES_ATTACK_VECTOR_H
