#include <iostream>
#include <zconf.h>
#include <csignal>
#include "Parser.h"

void capture_signal(int);

const pid_t m_pid = getpid();

int main(const int argc, const char *argv[]) {
    Parser::show_banner();
    config conf{};
    Logger logger{Logger::Info};
    Parser parser{&conf, &logger};
    parser.parse_commandline(&argc, argv);
    Validator validator(&conf);
    if(validator.Validate()){
        signal(SIGINT, capture_signal);
        Doser doser(&conf, &logger);
        doser.run();
    }else{
        logger.Log("Invalid Configuration", logger.Error);
        Parser::help();
    }

    return 0;
}

void capture_signal(int) {
    if (getpid() == m_pid) {
        std::cout << "\nShutting down..." << std::endl;
        usleep(1000000);
    }
    exit(EXIT_SUCCESS);
}