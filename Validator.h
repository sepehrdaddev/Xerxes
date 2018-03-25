#ifndef XERXES_VALIDATOR_H
#define XERXES_VALIDATOR_H

#include "Configuration.h"

class Validator {
public:
    Validator();

    explicit Validator(const config *conf);
    bool Validate();
    static bool isValidNumber(char const *num);
private:
    const config *conf;
    bool isValidWebsite();
    bool isValidPort();
    bool isValidConfig();
    bool isValidHostname();
};


#endif //XERXES_VALIDATOR_H
