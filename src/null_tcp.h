#ifndef XERXES_NULL_TCP_H
#define XERXES_NULL_TCP_H

#include "tcp_flood.h"

class null_tcp : public tcp_flood {
public:
    explicit null_tcp(std::shared_ptr<Config> config);

    ~null_tcp() override = default;
protected:
    int gen_hdr(std::string& string) override;
};


#endif //XERXES_NULL_TCP_H
