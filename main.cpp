#include <iostream>
#include <csignal>
#include <unistd.h>
#include <memory>
#include "Headers/Parser.hpp"

const pid_t m_pid = getpid();

void exiting(int){
    if (getpid() == m_pid) {
        std::cout << "Shutting down...\n";
        usleep(1000000);
    }
    exit(EXIT_SUCCESS);
}

void broke(int){
    // pass
}

void init_signals(){
    signal(SIGINT, &exiting);
    signal(SIGABRT, &exiting);
    signal(SIGTERM, &exiting);
    signal(SIGTSTP, &exiting);
    signal(SIGPIPE, &broke);
}

int main(const int argc, const char *argv[]) {
    init_signals();

    Parser::show_banner();
    auto config = std::make_shared<Config>();
    std::unique_ptr<Parser> parser{new Parser(config)};
    parser->parse_commandline(argc, argv);
    std::unique_ptr<Validator> validator{new Validator(config)};
    if(validator->Validate()){
        std::unique_ptr<Engine> engine{new Engine(config)};
        engine->run();
    }else{
        config->logger->Log("Invalid Configuration", Logger::Error);
        Parser::help();
    }

    return 0;
}