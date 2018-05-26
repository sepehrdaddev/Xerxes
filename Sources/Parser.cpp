#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>

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

    std::vector<std::string> arguments{"-h", "-nu", "-nt", "-fu", "-ft", "-r", "-ru", "-rh", "-w", "-s", "-ss", "-su",
                                       "-sy", "-sa", "-sr", "-sg", "-sp", "-sf", "-i", "-b", "-be", "-td", "-ld", "-q",
                                       "-qq", "-v", "-vv", "-target", "-port", "-T", "-C", "-D", "-help", "-version",
                                       "-rs", "-rp", "-B", "-sm"};

    for(int i = 1; i < argc; i++){
        for(int x = 0; x < arguments.size(); x++){
            switch(strcmp(argv[i], arguments[x].c_str())){
                case 0:{
                    switch(x){
                        case 0:{
                            conf->vector = Config::HTTP;
                            conf->protocol = Config::TCP;
                            break;
                        }
                        case 1:{
                            conf->vector = Config::NullUDP;
                            conf->protocol = Config::UDP;
                            break;
                        }
                        case 2:{
                            conf->vector = Config::NullTCP;
                            conf->protocol = Config::TCP;
                            break;
                        }
                        case 3:{
                            conf->vector = Config::UDPFlood;
                            conf->protocol = Config::UDP;
                            break;
                        }
                        case 4:{
                            conf->vector = Config::TCPFlood;
                            conf->protocol = Config::TCP;
                            break;
                        }
                        case 5:{
                            conf->vector = Config::Rudy;
                            conf->protocol = Config::TCP;
                            conf->delay = 10000000;
                            break;
                        }
                        case 6:{
                            conf->RandomizeUserAgent = true;
                            break;
                        }
                        case 7:{
                            conf->RandomizeHeader = true;
                            break;
                        }
                        case 8:{
                            conf->GetResponse = true;
                            break;
                        }
                        case 9:{
                            conf->vector = Config::Slowloris;
                            conf->protocol = Config::TCP;
                            conf->delay = 10000000;
                            break;
                        }
                        case 10:{
                            conf->UseSSL = true;
                            break;
                        }
                        case 11:{
                            conf->vector = Config::SpoofedUDP;
                            break;
                        }
                        case 12:{
                            conf->vector = Config::SpoofedSyn;
                            break;
                        }
                        case 13:{
                            conf->vector = Config::SpoofedAck;
                            break;
                        }
                        case 14:{
                            conf->vector = Config::SpoofedRST;
                            break;
                        }
                        case 15:{
                            conf->vector = Config::SpoofedURG;
                            break;
                        }
                        case 16:{
                            conf->vector = Config::SpoofedPUSH;
                            break;
                        }
                        case 17:{
                            conf->vector = Config::SpoofedFin;
                            break;
                        }
                        case 18:{
                            conf->vector = Config::ICMPFlood;
                            break;
                        }
                        case 19:{
                            conf->vector = Config::Blacknurse;
                            break;
                        }
                        case 20:{
                            conf->vector = Config::Beast;
                            break;
                        }
                        case 21:{
                            conf->vector = Config::TearDrop;
                            break;
                        }
                        case 22:{
                            conf->vector = Config::Land;
                            break;
                        }
                        case 23:{
                            conf->logger->setLevel(Logger::Error);
                            break;
                        }
                        case 24:{
                            conf->logger->setLevel(Logger::None);
                            break;
                        }
                        case 25:{
                            conf->logger->setLevel(Logger::Warning);
                            break;
                        }
                        case 26:{
                            conf->logger->setLevel(Logger::Info);
                            break;
                        }
                        case 27:{
                            conf->website = static_cast<std::string>(argv[i+1]);
                            break;
                        }
                        case 28:{
                            conf->port = static_cast<std::string>(argv[i+1]);
                            break;
                        }
                        case 29:{
                            if(Validator::isValidNumber(argv[i+1])){
                                conf->THREADS = static_cast<int>(strtol(argv[i+1], nullptr, 10));
                            }
                            break;
                        }
                        case 30:{
                            if(Validator::isValidNumber(argv[i+1])){
                                conf->CONNECTIONS = static_cast<int>(strtol(argv[i+1], nullptr, 10));
                            }
                            break;
                        }
                        case 31:{
                            if(Validator::isValidNumber(argv[i+1])){
                                conf->delay = static_cast<int>(strtol(argv[i+1], nullptr, 10));
                            }
                            break;
                        }
                        case 32:{
                            help();
                            break;
                        }
                        case 33:{
                            exit(EXIT_SUCCESS);
                        }
                        case 34:{
                            conf->RandomizeSource = true;
                            break;
                        }
                        case 35:{
                            conf->RandomizePort = true;
                            break;
                        }
                        case 36:{
                            conf->broadcast = static_cast<std::string>(argv[i+1]);
                            break;
                        }
                        case 37:{
                            conf->vector = Config::Smurf;
                            break;
                        }
                        default:break;
                    }
                    break;
                }
                default:break;
            }
        }
    }
    check_root();
    getUserAgents();
}

Parser::Parser() = default;

Parser::Parser(std::shared_ptr<Config> conf) : conf{conf}{

}

void Parser::check_root() {
    if(getuid()){
        conf->logger->Log("You need to be root", Logger::Error);
        exit(EXIT_FAILURE);
    }
}

void Parser::getUserAgents() {
    conf->useragents->push_back("Wget/1.16 (linux-gnu/Xerxes)");
    std::ifstream filestream("useragents");
    std::string line{};
    if(filestream.good() & filestream.is_open()){
        while(getline(filestream, line)){
            conf->useragents->push_back(line);
        }
        filestream.close();
    }else{
        conf->logger->Log("Unable to find useragents file", Logger::Warning);
    }
}
