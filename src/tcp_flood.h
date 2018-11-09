#ifndef XERXES_TCP_FLOOD_H
#define XERXES_TCP_FLOOD_H

#include <memory>
#include <string>

#include "Vector.h"
#include "config.h"

class tcp_flood : public Vector{

public:
    explicit tcp_flood(std::shared_ptr<Config> config);
    ~tcp_flood() override = default;
    void run() override;

protected:
    virtual int gen_hdr(std::string& string);
};


#endif //XERXES_TCP_FLOOD_H
