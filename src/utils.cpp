#include "utils.hpp"
#include "stdafx.hpp"

#include <random>

namespace utils {

namespace randomizer {

int randomInt(int min, int max) {
  static std::mt19937 engine{};
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(engine);
}

void randomIP(std::string &src) {
  src = fmt::format("{}.{}.{}.{}", randomInt(1, 255), randomInt(0, 255),
                    randomInt(0, 255), randomInt(0, 255));
}

int randomPort() {
  static unsigned int init_s_port = 0;
  static unsigned int seq = 0;
  if (init_s_port == 0) {
    init_s_port = 1024 + (randomInt(0, 2000));
    return init_s_port;
  }
  ++seq;
  return (seq + init_s_port) % 65536;
}

void randomstr(std::string &src) {
  unsigned int string_length = randomInt(1, 30);
  src.clear();
  for (unsigned int i = 0; i < string_length; ++i)
    src += (static_cast<char>('0' + randomInt(0, 72)));
}

void random_vec(const std::vector<std::string> &vec, std::string &str) {
  str = vec[randomizer::randomInt(0, static_cast<int>(vec.size()) - 1)];
}
} // namespace randomizer

namespace validator {

bool valid_host(const std::string &host) {
  sockaddr_in sa{};
  return static_cast<bool>(inet_pton(AF_INET, host.c_str(), &(sa.sin_addr)));
}

bool valid_hostname(const std::string &hostname) {
  hostent *record{gethostbyname(hostname.c_str())};
  return record != nullptr;
}

bool valid_port(const std::string &port) {
  unsigned int temp =
      static_cast<unsigned int>(strtol(port.c_str(), nullptr, 10));
  return temp < 65535 && temp > 0;
}
} // namespace validator

void pause(const unsigned int dly) {
  std::this_thread::sleep_for(std::chrono::nanoseconds(dly));
}

int to_int(const std::string &str) {
  return static_cast<int>(strtol(str.c_str(), nullptr, 10));
}

unsigned short csum(unsigned short *buf, int len) {
  unsigned long sum{};

  sum = 0;
  while (len > 1) {
    sum += *buf++;
    len -= 2;
  }

  if (len == 1)
    sum += *buf;

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return static_cast<unsigned short>(~sum);
}

void daemonize() {
  pid_t pid = 0;
  pid = fork();
  if (pid < 0) {
    spdlog::get("logger")->error("fork failed!");
    exit(1);
  }
  if (pid > 0) {
    spdlog::get("logger")->info("daemon pid {}", pid);
    exit(0);
  }
  umask(0);
  if (setsid() < 0)
    exit(1);
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}
}; // namespace utils
