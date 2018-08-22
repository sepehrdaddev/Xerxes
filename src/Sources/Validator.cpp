#include <arpa/inet.h>
#include <netdb.h>
#include <utility>

#include "../Headers/Validator.hpp"

Validator::Validator() = default;

Validator::Validator(std::shared_ptr<Config> conf) : conf{std::move(conf)} {

}

bool Validator::isValidWebsite(){
    struct sockaddr_in sa{};
    return (inet_pton(AF_INET, conf->website.c_str(), &(sa.sin_addr))) || isValidHostname();
}

bool Validator::isValidPort() {
    return strtol(conf->port.c_str(), nullptr, 10) ||
           strtol(conf->port.c_str(), nullptr, 10) > 65535 ||
           strtol(conf->port.c_str(), nullptr, 10) < 0;
}

bool Validator::isValidNumber(char const *num) {
    return static_cast<bool>(strtol(num, nullptr, 10));
}

bool Validator::isValidConfig() {
    return !conf->website.empty() && !conf->port.empty() && conf->CONNECTIONS > 0 && conf->THREADS > 0;
}

bool Validator::Validate() {
    switch(conf->vector){
        case Config::Smurf:
            return isValidConfig() && isValidWebsite() && isValidPort() && isValidBroadcast();
        default:
            return isValidConfig() && isValidWebsite() && isValidPort();
    }
}

bool Validator::isValidHostname(){
    hostent *record = gethostbyname(conf->website.c_str());
    return record != nullptr;
}

bool Validator::isValidBroadcast() {
    struct sockaddr_in sa{};
    return static_cast<bool>(inet_pton(AF_INET, conf->website.c_str(), &(sa.sin_addr)));
}
