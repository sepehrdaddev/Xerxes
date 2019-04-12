#include <algorithm>
#include <csignal>
#include <iostream>
#include <memory>
#include <string>

#include <args.hxx>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "engine.h"
#include "utils.h"

const char *__author__ = "Sepehrdad Sh";
const char *__license__ = "GPLv3";
const char *__version__ = "2.0beta";
const char *__project__ = "Xerxes enhanced";

std::vector<std::string> __str_vectors__{
    "NULL TCP",   "NULL UDP",   "TCP Flood", "UDP Flood", "HTTP Flood",
    "ICMP Flood", "SYN Flood",  "ACK Flood", "FIN Flood", "Spoofed UDP Flood",
    "Teardrop",   "Blacknurse", "Land",      "Smurf"};

void version() { printf("%s v%s\n", __project__, __version__); }

void banner() { printf("--==[ %s by %s ]==--\n\n", __project__, __author__); }

void print_vectors() {
  puts("available vectors:");

  for (long unsigned int i = 0; i < __str_vectors__.size(); ++i)
    printf("    > %ld  - %s\n", i, __str_vectors__[i].c_str());
}

void exit_signal(int) { exit(EXIT_SUCCESS); }

void broke(int) {
  // pass
}

void init_signals() {
  signal(SIGINT, &exit_signal);
  signal(SIGABRT, &exit_signal);
  signal(SIGTERM, &exit_signal);
  signal(SIGTSTP, &exit_signal);
  signal(SIGPIPE, &broke);
}

int main(int argc, const char *argv[]) {
  banner();

  args::ArgumentParser parser("Xerxes dos tool enhanced");

  args::HelpFlag help(parser, "help", "display this help menu", {'H', "help"});

  args::Flag ver(parser, "version", "display version", {'V', "version"});

  args::Flag vects(parser, "available vectors", "display available vectors",
                   {"vecs"});

  args::ValueFlag<std::string> rhost(parser, "rhost",
                                     "remote host address [default 127.0.0.1]",
                                     {'h', "rhost"}, "127.0.0.1");

  args::ValueFlag<std::string> rport(
      parser, "rport", "remote host port [default 80]", {'p', "rport"}, "80");

  args::ValueFlag<std::string> bcast(parser, "bcast",
                                     "broadcast address [default 127.0.0.1]",
                                     {'b', "bcast"}, "127.0.0.1");

  args::ValueFlag<int> vec(parser, "vector", "attack vector [default 0]",
                           {'v', "vec"}, 0);

  args::ValueFlag<int> dly(parser, "delay", "attack delay [default 1 ns]",
                           {'d', "dly"}, 1);

  args::ValueFlag<int> trds(parser, "threads", "number of threads [default 10]",
                            {'t', "trds"}, 10);

  args::ValueFlag<int> conn(parser, "connections",
                            "number of connections [default 25]", {'c', "conn"},
                            25);

  args::Flag tls(parser, "enable tls", "enable tls", {"tls"});

  args::Flag randomize_host(parser, "randomize lhost",
                            "enable local host randomization", {"rand-lhost"});

  args::Flag randomize_port(parser, "randomize lport",
                            "enable local port randomization", {"rand-lport"});

  spdlog::stdout_color_mt("logger");
  spdlog::set_pattern("[%^%l%$] %v");

  try {
    parser.ParseCLI(argc, argv);
  } catch (args::Help &) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError &e) {
    spdlog::get("logger")->error("can't parse arguments: {0}", e.what());
    return -1;
  }

  if (argc < 2) {
    std::cout << parser.Help();
    return 0;
  } else if (ver) {
    version();
    return 0;
  } else if (vects) {
    print_vectors();
    return 0;
  }

  init_signals();
  std::shared_ptr<Config> config = std::make_shared<Config>();
  utils::set_dly(args::get(dly), &config->time);
  config->rhost = args::get(rhost);
  config->rport = args::get(rport);
  config->bcast = args::get(bcast);
  config->vec = args::get(vec);
  config->conn = args::get(conn);
  config->trds = args::get(trds);
  config->tls = tls;
  config->rand_lhost = randomize_host;
  config->rand_lport = randomize_port;

  if (!(utils::validator::valid_host(config->rhost)) ||
      !(utils::validator::valid_hostname(config->rhost))) {
    spdlog::get("logger")->error("Invalid rhost address");
    return -1;
  } else if (!(utils::validator::valid_port(config->rport))) {
    spdlog::get("logger")->error("Invalid rport number");
    return -1;
  } else if (config->conn <= 0) {
    spdlog::get("logger")->error("Invalid connections number");
    return -1;
  } else if (config->trds <= 0) {
    spdlog::get("logger")->error("Invalid threads number");
    return -1;
  }

  engine eng{config};

  return 0;
}
