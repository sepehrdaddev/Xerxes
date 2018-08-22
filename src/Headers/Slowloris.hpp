#ifndef XERXES_SLOWLORIS_H
#define XERXES_SLOWLORIS_H

#include "Configuration.hpp"
#include "Randomizer.hpp"
#include "Http_Flood.hpp"

class Slowloris : public Http_Flood {
public:
    explicit Slowloris(std::shared_ptr<Config> conf);

private:
    void attack() override;
    void attack_ssl() override;
    void init_header(httphdr *header) override;
};


#endif //XERXES_SLOWLORIS_H
