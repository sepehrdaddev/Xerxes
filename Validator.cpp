#include "Validator.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

Validator::Validator() = default;

Validator::Validator(const config *conf) : conf{conf} {

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
    return isValidConfig() && isValidWebsite() && isValidPort();
}

bool Validator::isValidHostname(){
    hostent *record = gethostbyname(conf->website.c_str());
    return record != nullptr;
}
