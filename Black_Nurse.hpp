#ifndef XERXES_BLACK_NURSE_H
#define XERXES_BLACK_NURSE_H

#include "Spoofed_Flood.hpp"

class Black_Nurse : public Spoofed_Flood {
public:
    Black_Nurse(const config *conf, Logger *logger);

private:
    void attack(const int *id) override;

};

#endif //XERXES_BLACK_NURSE_H
