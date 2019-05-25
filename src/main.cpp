#include "engine.h"
#include "stdafx.h"
#include "utils.h"

#include <algorithm>
#include <args.hxx>
#include <csignal>

const std::string __author__ = "Sepehrdad Sh", __license__ = "GPLv3",
                  __version__ = "v2.0beta", __project__ = "Xerxes enhanced";

void version() { fmt::print("{} {}\n", __project__, __version__); }

void banner() {
  fmt::print(fg(fmt::color::red) | fmt::emphasis::bold,
             "--==[ {} by {} ]==--\n\n", __project__, __author__);
}

void print_vectors() {
  static const std::vector<std::string> __str_vectors__{
      "NULL TCP",      "NULL UDP",
      "TCP Flood",     "UDP Flood",
      "HTTP Flood",    "ICMP Flood",
      "SYN Flood",     "ACK Flood",
      "FIN Flood",     "Spoofed UDP Flood",
      "Teardrop",      "Blacknurse",
      "Land",          "Smurf",
      "ACK PSH Flood", "RST Flood",
      "SYN ACK Flood", "XMAS Flood"};

  fmt::print("available vectors:\n");

  for (size_t i = 0; i < __str_vectors__.size(); ++i)
    fmt::print("    > {}   - {}\n", i, __str_vectors__[i]);
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

  args::Flag daemonize(parser, "daemonize", "daemonize", {'D', "daemonize"});

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

  Config::get().rhost = args::get(rhost);
  Config::get().rport = args::get(rport);
  Config::get().bcast = args::get(bcast);
  Config::get().tls = tls;
  Config::get().rand_lhost = randomize_host;
  Config::get().rand_lport = randomize_port;

  Config::get().vec = [&]() {
    if (args::get(vec) < 0) {
      spdlog::get("logger")->error("Invalid attack vector");
      exit(-1);
    }
    return args::get(vec);
  }();

  Config::get().conn = [&]() {
    if (args::get(conn) <= 0) {
      spdlog::get("logger")->error("Invalid connections number");
      exit(-1);
    }
    return args::get(conn);
  }();

  Config::get().trds = [&]() {
    if (args::get(trds) <= 0) {
      spdlog::get("logger")->error("Invalid threads number");
      exit(-1);
    }
    return args::get(trds);
  }();

  Config::get().dly = [&]() -> unsigned int {
    if (args::get(dly) <= 0) {
      spdlog::get("logger")->error(
          "delay cannot be less than 1 ns, setting to default");
      return 1;
    }
    return args::get(dly);
  }();

  if (!(utils::validator::valid_host(Config::get().rhost)) ||
      !(utils::validator::valid_hostname(Config::get().rhost))) {
    spdlog::get("logger")->error("Invalid rhost address");
    return -1;
  } else if (!(utils::validator::valid_port(Config::get().rport))) {
    spdlog::get("logger")->error("Invalid rport number");
    return -1;
  }

  Engine::get().start();

  return 0;
}
