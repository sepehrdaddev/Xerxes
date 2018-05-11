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
                                       "-rs", "-rp"};

    for(int i = 1; i < argc; i++){
        for(int x = 0; x < arguments.size(); x++){
            switch(strcmp(argv[i], arguments[x].c_str())){
                case 0:{
                    switch(x){
                        case 0:{
                            conf->vector = config::HTTP;
                            conf->protocol = config::TCP;
                            break;
                        }
                        case 1:{
                            conf->vector = config::NullUDP;
                            conf->protocol = config::UDP;
                            break;
                        }
                        case 2:{
                            conf->vector = config::NullTCP;
                            conf->protocol = config::TCP;
                            break;
                        }
                        case 3:{
                            conf->vector = config::UDPFlood;
                            conf->protocol = config::UDP;
                            break;
                        }
                        case 4:{
                            conf->vector = config::TCPFlood;
                            conf->protocol = config::TCP;
                            break;
                        }
                        case 5:{
                            conf->vector = config::Rudy;
                            conf->protocol = config::TCP;
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
                            conf->vector = config::Slowloris;
                            conf->protocol = config::TCP;
                            conf->delay = 10000000;
                            break;
                        }
                        case 10:{
                            conf->UseSSL = true;
                            break;
                        }
                        case 11:{
                            conf->vector = config::SpoofedUDP;
                            break;
                        }
                        case 12:{
                            conf->vector = config::SpoofedSyn;
                            break;
                        }
                        case 13:{
                            conf->vector = config::SpoofedAck;
                            break;
                        }
                        case 14:{
                            conf->vector = config::SpoofedRST;
                            break;
                        }
                        case 15:{
                            conf->vector = config::SpoofedURG;
                            break;
                        }
                        case 16:{
                            conf->vector = config::SpoofedPUSH;
                            break;
                        }
                        case 17:{
                            conf->vector = config::SpoofedFin;
                            break;
                        }
                        case 18:{
                            conf->vector = config::ICMPFlood;
                            break;
                        }
                        case 19:{
                            conf->vector = config::Blacknurse;
                            break;
                        }
                        case 20:{
                            conf->vector = config::Beast;
                            break;
                        }
                        case 21:{
                            conf->vector = config::TearDrop;
                            break;
                        }
                        case 22:{
                            conf->vector = config::Land;
                            break;
                        }
                        case 23:{
                            logger->setLevel(Logger::Error);
                            break;
                        }
                        case 24:{
                            logger->setLevel(Logger::None);
                            break;
                        }
                        case 25:{
                            logger->setLevel(Logger::Warning);
                            break;
                        }
                        case 26:{
                            logger->setLevel(Logger::Info);
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

Parser::Parser(config *conf, Logger *logger) : conf{conf}, logger{logger}{

}

void Parser::getUserAgents() {
    std::ifstream filestream("useragents");
    std::string line{};
    if(filestream.good() & filestream.is_open()){
        while(getline(filestream, line)){
            conf->useragents.push_back(line);
        }
        filestream.close();
    }else{
        logger->Log("Unable to find useragents file", Logger::Warning);
    }
}

void Parser::check_root() {
    if(getuid()){
        logger->Log("You need to be root", Logger::Error);
        exit(EXIT_FAILURE);
    }
}
