#include <iostream>
#include <fstream>
#include <unistd.h>
#include <utility>
#include <algorithm>

#include "../Headers/Parser.hpp"
#include "../Headers/Logging.hpp"

void Parser::help() {
    const char* msg{"Usage: ./Xerxes -target <ip or url> -port <port> <options>\n"
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
                                "                -D               set delay in microseconds\n"
                                "                -T               set number of threads\n"
                                "                -C               set number of connections per thread\n"
                                "                -B               set broadcast address\n"
                                "                -target          set target ip or url\n"
                                "                -port            set target port number\n"
                                "                -help            show help\n"
                                "                -version         show version\n"
    };
    fprintf(stdout, "%s\n", msg);
    exit(EXIT_SUCCESS);
}

void Parser::show_banner() {
    fprintf(stdout, "Xerxes version %s %s (Xerxes - Revised)\n", Version, Time);
}

void Parser::parse_commandline(int argc, const char *argv[]) {
    init_arguments();

    for(int i = 1; i < argc; i++){
        if(arguments[argv[i]]){
            if(argv[i+1]){
                const char *next = argv[i+1];
                arguments[argv[i]](next);
            }else{
                arguments[argv[i]](" ");
            }
        }
    }
    check_root();
    getUserAgents();
}

Parser::Parser() = default;

Parser::Parser(std::shared_ptr<Config> conf) : conf{std::move(conf)}{

}

void Parser::check_root() {
    if(getuid()){
        print_error("You need to be root");
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
        print_warning("Unable to find useragents file");
    }
}

void Parser::init_arguments() {
    // Attack Vector
    arguments["-h"] = [&](const char*){conf->vector = Config::HTTP;conf->protocol = Config::TCP;};
    arguments["-nu"] = [&](const char*){conf->vector = Config::NullUDP;conf->protocol = Config::UDP;};
    arguments["-nt"] = [&](const char*){conf->vector = Config::NullTCP;conf->protocol = Config::TCP;};
    arguments["-fu"] = [&](const char*){conf->vector = Config::UDPFlood;conf->protocol = Config::UDP;};
    arguments["-ft"] = [&](const char*){conf->vector = Config::TCPFlood;conf->protocol = Config::TCP;};
    arguments["-r"] = [&](const char*){conf->vector = Config::Rudy;conf->protocol = Config::TCP;conf->delay = 10000000;};
    arguments["-s"] = [&](const char*){conf->vector = Config::Slowloris;conf->protocol = Config::TCP;conf->delay = 10000000;};
    arguments["-su"] = [&](const char*){conf->vector = Config::SpoofedUDP;};
    arguments["-sy"] = [&](const char*){conf->vector = Config::SpoofedSyn;};
    arguments["-sa"] = [&](const char*){conf->vector = Config::SpoofedAck;};
    arguments["-sr"] = [&](const char*){conf->vector = Config::SpoofedRST;};
    arguments["-sg"] = [&](const char*){conf->vector = Config::SpoofedURG;};
    arguments["-sp"] = [&](const char*){conf->vector = Config::SpoofedPUSH;};
    arguments["-sf"] = [&](const char*){conf->vector = Config::SpoofedFin;};
    arguments["-i"] = [&](const char*){conf->vector = Config::ICMPFlood;};
    arguments["-b"] = [&](const char*){conf->vector = Config::Blacknurse;};
    arguments["-be"] = [&](const char*){conf->vector = Config::Beast;};
    arguments["-td"] = [&](const char*){conf->vector = Config::TearDrop;};
    arguments["-ld"] = [&](const char*){conf->vector = Config::Land;};
    arguments["-sm"] = [&](const char*){conf->vector = Config::Smurf;};

    // Randomization
    arguments["-ru"] = [&](const char*){conf->RandomizeUserAgent = true;};
    arguments["-rh"] = [&](const char*){conf->RandomizeHeader = true;};
    arguments["-rs"] = [&](const char*){conf->RandomizeSource = true;};
    arguments["-rp"] = [&](const char*){conf->RandomizePort = true;};

    // Other
    arguments["-w"] = [&](const char*){conf->GetResponse = true;};
    arguments["-ss"] = [&](const char*){conf->UseSSL = true;};
    arguments["-help"] = [&](const char*){help();};
    arguments["-version"] = [&](const char*){exit(EXIT_SUCCESS);};

    // Configuration
    arguments["-target"] = [&](const char* target){conf->website = static_cast<std::string>(target);};
    arguments["-port"] = [&](const char* port){conf->port = static_cast<std::string>(port);};
    arguments["-T"] = [&](const char* thread){
        if(Validator::isValidNumber(thread)){
            conf->THREADS = static_cast<int>(strtol(thread, nullptr, 10));
        }
    };
    arguments["-C"] = [&](const char* connections){
        if(Validator::isValidNumber(connections)){
            conf->CONNECTIONS = static_cast<int>(strtol(connections, nullptr, 10));
        }
    };
    arguments["-D"] = [&](const char* delay){
        if(Validator::isValidNumber(delay)){
            conf->delay = static_cast<int>(strtol(delay, nullptr, 10));}
    };
    arguments["-B"] = [&](const char* broadcast){conf->broadcast = static_cast<std::string>(broadcast);};
}
