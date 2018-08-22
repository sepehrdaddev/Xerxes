#include <iostream>
#include <csignal>
#include <unistd.h>
#include <memory>
#include <ctime>

#include "../Headers/Parser.hpp"
#include "../Headers/Logging.hpp"

const pid_t m_pid = getpid();
std::function<void()> show_stat;


void exit_signal(int){
    if (getpid() == m_pid) {
        fprintf(stdout, "%s\n", "Shutting down...");
        usleep(100000);
    }
    exit(EXIT_SUCCESS);
}

void on_exit(){
    if (getpid() == m_pid) {
        show_stat();
    }
}

void broke(int){
    // pass
}

void init_signals(){
    signal(SIGINT, &exit_signal);
    signal(SIGABRT, &exit_signal);
    signal(SIGTERM, &exit_signal);
    signal(SIGTSTP, &exit_signal);
    signal(SIGPIPE, &broke);
    atexit(on_exit);
}

int main(const int argc, const char *argv[]) {
    init_signals();

    Parser::show_banner();
    auto config = std::make_shared<Config>();
    show_stat = [&]{config->show_stat();};
    auto parser = std::make_unique<Parser>(config);
    parser->parse_commandline(argc, argv);
    auto validator = std::make_unique<Validator>(config);
    if(validator->Validate()){
        auto engine = std::make_unique<Engine>(config);
        engine->run();
    }else{
        print_error("Invalid Configuration");
        Parser::help();
    }

    return 0;
}
