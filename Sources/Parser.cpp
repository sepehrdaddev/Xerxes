#include <iostream>
#include <fstream>
#include <unistd.h>
#include <utility>
#include <algorithm>

#include "../Headers/Parser.hpp"

void Parser::help() {
    std::string message{"Usage: ./Xerxes -target <ip or url> -port <port> <options>\n"
                        "\nOptions:\n"
                                "                -h               set attack vector to HTTP\n"
                                "                -i               set attack vector to Spoofed ICMP Flood\n"
                                "                -r               set attack vector to Rudy\n"
                                "                -s               set attack vector to Slowloris\n"
                                "                -b               set attack vector to Black Nurse\n"
                                "                -be              set attack vector to Beast\n"
                                "                -nu              set attack vector to NULLUDP\n"
                                "                -nt              set attack vector to NULLTCP\n"
                                "                -ft              set attack vector to TCPFlood\n"
                                "                -fu              set attack vector to UDPFlood\n"
                                "                -su              set attack vector to Spoofed UDP Flood\n"
                                "                -sy              set attack vector to Spoofed Syn Flood\n"
                                "                -sa              set attack vector to Spoofed Ack Flood\n"
                                "                -sr              set attack vector to Spoofed Rst Flood\n"
                                "                -sg              set attack vector to Spoofed Urg Flood\n"
                                "                -sp              set attack vector to Spoofed Push Flood\n"
                                "                -sf              set attack vector to Spoofed Fin Flood\n"
                                "                -td              set attack vector to Teardrop\n"
                                "                -ld              set attack vector to Land\n"
                                "                -sm              set attack vector to Smurf\n"
                                "                -ss              enable SSL\n"
                                "                -w               wait for response\n"
                                "                -rh              randomize HTTP Header\n"
                                "                -ru              randomize HTTP UserAgent\n"
                                "                -rs              randomize Source IP\n"
                                "                -rp              randomize Source Port\n"
                                "                -qq              set verbosity to quiet quiet\n"
                                "                -q               set verbosity to quiet\n"
                                "                -v               set verbosity to verbose\n"
                                "                -vv              set verbosity to very verbose\n"
                                "                -D               set delay in microseconds\n"
                                "                -T               set number of threads\n"
                                "                -C               set number of connections per thread\n"
                                "                -B               set broadcast address\n"
                                "                -target          set target ip or url\n"
                                "                -port            set target port number\n"
                                "                -help            show help\n"
                                "                -version         show version\n"
    };
    std::cout << message << std::endl;
    exit(EXIT_SUCCESS);
}

void Parser::show_banner() {
    std::cout << "Xerxes version " << Version << " " << Time << " (Xerxes - Revised)" << std::endl;
}

void Parser::parse_commandline(int argc, const char *argv[]) {
    check_root();
    init_arguments();

    for(int i = 1; i < argc; i++){
        if(arguments[argv[i]]){
            if(argv[i+1]){
                std::string next = std::string{argv[i+1]};
                arguments[argv[i]](next);
            }else{
                arguments[argv[i]]((std::string&)" ");
            }
        }
    }
    getUserAgents();
}

Parser::Parser() = default;

Parser::Parser(std::shared_ptr<Config> conf) : conf{std::move(conf)}{

}

void Parser::check_root() {
    if(getuid()){
        conf->logger->Log("You need to be root", Logger::Error);
        exit(EXIT_FAILURE);
    }
}

void Parser::getUserAgents() {
    conf->useragents->emplace_back("Wget/1.16 (linux-gnu/Xerxes)");
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
        conf->logger->Log("Unable to find useragents file", Logger::Warning);
    }
}

void Parser::init_arguments() {
    // Attack Vector
    arguments["-h"] = [&](std::string&){conf->vector = Config::HTTP;conf->protocol = Config::TCP;};
    arguments["-nu"] = [&](std::string&){conf->vector = Config::NullUDP;conf->protocol = Config::UDP;};
    arguments["-nt"] = [&](std::string&){conf->vector = Config::NullTCP;conf->protocol = Config::TCP;};
    arguments["-fu"] = [&](std::string&){conf->vector = Config::UDPFlood;conf->protocol = Config::UDP;};
    arguments["-ft"] = [&](std::string&){conf->vector = Config::TCPFlood;conf->protocol = Config::TCP;};
    arguments["-r"] = [&](std::string&){conf->vector = Config::Rudy;conf->protocol = Config::TCP;conf->delay = 10000000;};
    arguments["-s"] = [&](std::string&){conf->vector = Config::Slowloris;conf->protocol = Config::TCP;conf->delay = 10000000;};
    arguments["-su"] = [&](std::string&){conf->vector = Config::SpoofedUDP;};
    arguments["-sy"] = [&](std::string&){conf->vector = Config::SpoofedSyn;};
    arguments["-sa"] = [&](std::string&){conf->vector = Config::SpoofedAck;};
    arguments["-sr"] = [&](std::string&){conf->vector = Config::SpoofedRST;};
    arguments["-sg"] = [&](std::string&){conf->vector = Config::SpoofedURG;};
    arguments["-sp"] = [&](std::string&){conf->vector = Config::SpoofedPUSH;};
    arguments["-sf"] = [&](std::string&){conf->vector = Config::SpoofedFin;};
    arguments["-i"] = [&](std::string&){conf->vector = Config::ICMPFlood;};
    arguments["-b"] = [&](std::string&){conf->vector = Config::Blacknurse;};
    arguments["-be"] = [&](std::string&){conf->vector = Config::Beast;};
    arguments["-td"] = [&](std::string&){conf->vector = Config::TearDrop;};
    arguments["-ld"] = [&](std::string&){conf->vector = Config::Land;};
    arguments["-sm"] = [&](std::string&){conf->vector = Config::Smurf;};

    // Randomization
    arguments["-ru"] = [&](std::string&){conf->RandomizeUserAgent = true;};
    arguments["-rh"] = [&](std::string&){conf->RandomizeHeader = true;};
    arguments["-rs"] = [&](std::string&){conf->RandomizeSource = true;};
    arguments["-rp"] = [&](std::string&){conf->RandomizePort = true;};

    // Other
    arguments["-w"] = [&](std::string&){conf->GetResponse = true;};
    arguments["-ss"] = [&](std::string&){conf->UseSSL = true;};
    arguments["-help"] = [&](std::string&){help();};
    arguments["-version"] = [&](std::string&){exit(EXIT_SUCCESS);};

    // Logging
    arguments["-q"] = [&](std::string&){conf->logger->setLevel(Logger::Error);};
    arguments["-qq"] = [&](std::string&){conf->logger->setLevel(Logger::None);};
    arguments["-v"] = [&](std::string&){conf->logger->setLevel(Logger::Warning);};
    arguments["-vv"] = [&](std::string&){conf->logger->setLevel(Logger::Info);};

    // Configuration
    arguments["-target"] = [&](std::string& target){conf->website = static_cast<std::string>(target);};
    arguments["-port"] = [&](std::string& port){conf->port = static_cast<std::string>(port);};
    arguments["-T"] = [&](std::string& thread){
        if(Validator::isValidNumber(thread.c_str())){
            conf->THREADS = static_cast<int>(strtol(thread.c_str(), nullptr, 10));
        }
    };
    arguments["-C"] = [&](std::string& connections){
        if(Validator::isValidNumber(connections.c_str())){
            conf->CONNECTIONS = static_cast<int>(strtol(connections.c_str(), nullptr, 10));
        }
    };
    arguments["-D"] = [&](std::string& delay){
        if(Validator::isValidNumber(delay.c_str())){
            conf->delay = static_cast<int>(strtol(delay.c_str(), nullptr, 10));}
    };
    arguments["-B"] = [&](std::string& broadcast){conf->broadcast = static_cast<std::string>(broadcast);};
}
