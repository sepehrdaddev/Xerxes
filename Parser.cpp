#include <iostream>
#include <fstream>
#include <vector>
#include "Parser.h"

void Parser::help() {
    std::string message{"Usage: ./Xerxes <website> <port> <threads> <connections> <options>\n"
                        "\nOptions:\n"
                                "                -h+              set attack vector to HTTP plus\n"
                                "                -h               set attack vector to HTTP\n"
                                "                -n               set attack vector to NULL\n"
                                "                -n+              set attack vector to NULL plus\n"
                                "                -t               set attack protocol to TCP\n"
                                "                -u               set attack protocol to UDP\n"
                                "                -qq              set verbosity to quiet quiet\n"
                                "                -q               set verbosity to quiet\n"
                                "                -v               set verbosity to verbose\n"
                                "                -vv              set verbosity to very verbose\n"
    };
    std::cout << message << std::endl;
    exit(EXIT_SUCCESS);
}

void Parser::show_banner() {
    const std::string version{"v0.0.2"};
    std::cout << "Xerxes - Revised " << version << std::endl;
}

void Parser::parse_commandline(const int *argc, const char *argv[]) {
    size_t i = 0;
    switch(*argc){
        case 8:
        case 7:
        case 6:
        case 5:{
            for (i = 1; i < *argc; i++) {
                switch(argv[i][0]){
                    case '-':{
                        switch(argv[i][1]){
                            case 'h':
                                if(argv[i][2] == '+'){
                                    conf->vector = config::HTTPPlus;
                                }else{
                                    conf->vector = config::HTTP;
                                }
                                break;
                            case 'n':
                                if(argv[i][2] == '+'){
                                    conf->vector = config::NullPlus;
                                }else{
                                    conf->vector = config::Null;
                                }
                                break;
                            case 't':
                                conf->protocol = config::TCP;
                                break;
                            case 'u':
                                conf->protocol = config::UDP;
                                break;
                            case 'v':
                                if(argv[i][2] == 'v'){
                                    logger->setLevel(Logger::Info);
                                }else{
                                    logger->setLevel(Logger::Warning);
                                }
                                break;
                            case 'q':
                                if(argv[i][2] == 'q'){
                                    logger->setLevel(Logger::None);
                                }else{
                                    logger->setLevel(Logger::Error);
                                }
                                break;
                            default:
                                help();
                        }
                        break;
                    }
                    default:{
                        conf->website = (std::string) argv[1];
                        conf->port = (std::string) argv[2];
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
            break;
        }
        default:
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