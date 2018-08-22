#ifndef XERXES_NULL_FLOOD_H
#define XERXES_NULL_FLOOD_H

#include "Configuration.hpp"
#include "Http_Flood.hpp"

class Null_Flood : public Http_Flood {
public:
    explicit Null_Flood(std::shared_ptr<Config> conf);

private:
    void attack() override;
    void attack_ssl() override;

};


#endif //XERXES_NULL_FLOOD_H
