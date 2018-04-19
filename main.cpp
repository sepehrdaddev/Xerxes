#include <iostream>
#include <csignal>
#include <unistd.h>
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

int main(const int argc, const char *argv[]) {
    signal(SIGINT, &exiting);
    signal(SIGABRT, &exiting);
    signal(SIGTERM, &exiting);
    signal(SIGTSTP, &exiting);
    signal(SIGPIPE, &broke);

    Parser::show_banner();
    config conf{};
    Logger logger{Logger::Info};
    Parser parser{&conf, &logger};
    parser.parse_commandline(argc, argv);
    Validator validator(&conf);
    if(validator.Validate()){
        Doser doser(&conf, &logger);
        doser.run();
    }else{
        logger.Log("Invalid Configuration", logger.Error);
        Parser::help();
    }

    return 0;
}