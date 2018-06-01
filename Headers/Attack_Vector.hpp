#ifndef XERXES_ATTACK_VECTOR_H
#define XERXES_ATTACK_VECTOR_H

#include "Configuration.hpp"

class Attack_Vector {
    friend class Spoofed_Flood;
    friend class Http_Flood;
    friend class Beast;

public:
    Attack_Vector() = default;
    explicit Attack_Vector(std::shared_ptr<Config> config);
    virtual void run();
    virtual ~Attack_Vector() = default;

protected:
    std::shared_ptr<Config> conf;
    virtual void pause();

private:
    virtual void attack() = 0;
};


#endif //XERXES_ATTACK_VECTOR_H
