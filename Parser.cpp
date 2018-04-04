#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.hpp"

void Parser::help() {
    std::string message{"Usage: ./Xerxes <website> <port> <threads> <connections> <options>\n"
                        "\nOptions:\n"
                                "                -h               set attack vector to HTTP\n"
                                "                -i               set attack vector to Spoofed ICMP Flood\n"
                                "                -s               set attack vector to Slowloris\n"
                                "                -nu              set attack vector to NULLUDP\n"
                                "                -nt              set attack vector to NULLTCP\n"
                                "                -ft              set attack vector to TCPFlood\n"
                                "                -fu              set attack vector to UDPFlood\n"
                                "                -su              set attack vector to Spoofed UDP Flood\n"
                                "                -st              set attack vector to Spoofed TCP Flood\n"
                                "                -ss              enable SSL\n"
                                "                -w               wait for hosts response\n"
                                "                -rh              randomize HTTP Header\n"
                                "                -ru              randomize HTTP UserAgent\n"
                                "                -qq              set verbosity to quiet quiet\n"
                                "                -q               set verbosity to quiet\n"
                                "                -v               set verbosity to verbose\n"
                                "                -vv              set verbosity to very verbose\n"
    };
    std::cout << message << std::endl;
    exit(EXIT_SUCCESS);
}

void Parser::show_banner() {
    const std::string version{"v0.0.7"};
    std::cout << "Xerxes - Revised " << version << std::endl;
}

void Parser::parse_commandline(const int *argc, const char *argv[]) {
    size_t i = 0;
    if(*argc >= 5){
        for (i = 1; i < *argc; i++){
            switch(argv[i][0]){
                case '-':{
                    switch(argv[i][1]){
                        case 'h':
                            conf->vector = config::HTTP;
                            conf->protocol = config::TCP;
                            break;
                        case 'n':
                            if(argv[i][2] == 'u'){
                                conf->vector = config::NullUDP;
                                conf->protocol = config::UDP;
                            }else if(argv[i][2] == 't'){
                                conf->vector = config::NullTCP;
                                conf->protocol = config::TCP;
                            }
                            break;
                        case 'f':
                            if(argv[i][2] == 'u'){
                                conf->vector = config::UDPFlood;
                                conf->protocol = config::UDP;
                            }else if(argv[i][2] == 't'){
                                conf->vector = config::TCPFlood;
                                conf->protocol = config::TCP;
                            }
                            break;
                        case 'r':
                            if(argv[i][2] == 'u'){
                                conf->RandomizeUserAgent = true;
                            }else if(argv[i][2] == 'h'){
                                conf->RandomizeHeader = true;
                            }
                            break;
                        case 'w':
                            conf->GetResponse = true;
                            break;
                        case 's':
                            switch(argv[i][2]){
                                case 's':
                                    conf->UseSSL = true;
                                    break;
                                case 'u':
                                    conf->vector = config::SpoofedUDP;
                                    break;
                                case 't':
                                    conf->vector = config::SpoofedTCP;
                                    break;
                                default:
                                    conf->vector = config::Slowloris;
                                    conf->protocol = config::TCP;
                                    conf->delay = 10000000;
                            }
                            break;
                        case 'q':
                            if(argv[i][2] == 'q'){
                                logger->setLevel(Logger::None);
                            }else{
                                logger->setLevel(Logger::Error);
                            }
                            break;
                        case 'v':
                            if(argv[i][2] == 'v'){
                                logger->setLevel(Logger::Info);
                            }else{
                                logger->setLevel(Logger::Warning);
                            }
                            break;
                        case 'i':
                            conf->vector = config::ICMPFlood;
                            break;
                        default:
                            help();
                    }
                    break;
                }
                default:{
                    conf->website = static_cast<std::string>(argv[1]);
                    conf->port = static_cast<std::string>(argv[2]);
                    if(Validator::isValidNumber(argv[3]) & Validator::isValidNumber(argv[4])){
                        conf->THREADS = static_cast<int>(strtol(argv[3], nullptr, 10));
                        conf->CONNECTIONS = static_cast<int>(strtol(argv[4], nullptr, 10));
                    }else{
                        help();
                    }
                }
            }
        }
        getUserAgents();
    }else{
        help();
    }
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