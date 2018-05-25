#ifndef XERXES_SLOWLORIS_H
#define XERXES_SLOWLORIS_H

#include "Configuration.hpp"
#include "Logger.hpp"
#include "Randomizer.hpp"
#include "Http_Flood.hpp"

class Slowloris : public Http_Flood {
public:
    explicit Slowloris(std::shared_ptr<Config> conf);

private:
    void attack(const int *id) override;
    void attack_ssl(const int *id) override;
    void init_header(std::string& header, bool keep_alive) override;
};


#endif //XERXES_SLOWLORIS_H
