#ifndef XERXES_VALIDATOR_H
#define XERXES_VALIDATOR_H

#include "Configuration.hpp"

class Validator {
public:
    Validator();

    explicit Validator(const Config *conf);
    bool Validate();
    static bool isValidNumber(char const *num);
private:
    const Config *conf;
    bool isValidWebsite();
    bool isValidPort();
    bool isValidConfig();
    bool isValidHostname();
    bool isValidBroadcast();
};


#endif //XERXES_VALIDATOR_H
