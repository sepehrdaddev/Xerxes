#define CATCH_CONFIG_MAIN

#include "stdafx.h"

#include <catch.hpp>
#include <regex>
#include <utils.h>

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

TEST_CASE("test utils random int") {
  std::vector<int> nums{};
  nums.reserve(10000);
  int min = -100000, max = 100000, rand;
  for (int i = 0; i < 10000; ++i) {
    rand = utils::randomizer::randomInt(min, max);
    REQUIRE(rand >= min);
    REQUIRE(rand <= max);
    nums.emplace_back(rand);
  }
  REQUIRE(std::adjacent_find(nums.begin(), nums.end()) == nums.end());
}

TEST_CASE("test utils random ip") {
  std::vector<std::string> ips{};
  ips.reserve(10000);
  std::string temp{};
  std::vector<std::string> temp_vec{};
  for (int i = 0; i < 10000; ++i) {
    std::regex re("^[1-9][0-9]{0,2}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$");
    utils::randomizer::randomIP(temp);
    temp_vec = split(temp, '.');
    for (auto &val : temp_vec) {
      REQUIRE(std::regex_match(temp, re));
      REQUIRE(utils::to_int(val) < 256);
    }

    ips.emplace_back(temp);
  }

  REQUIRE(std::adjacent_find(ips.begin(), ips.end()) == ips.end());
}

TEST_CASE("test utils random port") {
  std::vector<int> ports{};
  ports.reserve(10000);
  int port;
  bool is_continious = true;
  for (int i = 0; i < 10000; ++i) {
    port = utils::randomizer::randomPort();
    REQUIRE(port > 0);
    REQUIRE(port < 65536);
    ports.emplace_back(port);
  }

  for (unsigned long i = 0; i < ports.size(); ++i) {
    if ((i + 1 != ports.size()) && (ports[i] + 1 != ports[i + 1])) {
      is_continious = false;
    }
  }

  REQUIRE(std::adjacent_find(ports.begin(), ports.end()) == ports.end());
  REQUIRE(is_continious);
}

TEST_CASE("test utils random str") {
  std::vector<std::string> strs{};
  strs.reserve(10000);
  std::string temp{};
  for (int i = 0; i < 10000; ++i) {
    utils::randomizer::randomstr(temp);
    REQUIRE_FALSE(temp.empty());
    REQUIRE(temp.size() <= 30);
    strs.emplace_back(temp);
  }

  REQUIRE(std::adjacent_find(strs.begin(), strs.end()) == strs.end());
}

TEST_CASE("test utils random vector") {
  std::vector<std::string> strs{};
  strs.reserve(10000);
  std::string temp{};
  for (int i = 0; i < 10000; ++i) {
    utils::randomizer::randomstr(temp);
    strs.emplace_back(temp);
  }

  std::vector<std::string> random_strs{};
  strs.reserve(5000);

  for (int i = 0; i < 5000; ++i) {
    utils::randomizer::random_vec(strs, temp);
    random_strs.emplace_back(temp);
  }

  REQUIRE(std::adjacent_find(random_strs.begin(), random_strs.end()) ==
          random_strs.end());
}

TEST_CASE("test utils to_int") {
  for (int i = -10000; i < 10000; ++i) {
    REQUIRE(utils::to_int(std::to_string(i)) == i);
  }
}

TEST_CASE("test utils valid host") {
  std::vector<std::string> invalid_hosts{"random",
                                         "rand.rand",
                                         "192.aaa.2.5",
                                         "aaa.8.5.3",
                                         "7.4.ee.1",
                                         "1.1.1.a",
                                         "random.randm.random.rand"};

  for (auto &i : invalid_hosts)
    REQUIRE_FALSE(utils::validator::valid_host(i));

  std::vector<std::string> valid_hosts{"1.1.1.1", "8.8.8.8", "1.0.0.1",
                                       "8.8.4.4", "127.0.0.1"};

  for (auto &i : valid_hosts)
    REQUIRE(utils::validator::valid_host(i));
}

TEST_CASE("test utils valid hostname") {
  std::vector<std::string> valid_hostnames{"google.com", "yahoo.com",
                                           "microsoft.com", "duckduckgo.com"};
  for (auto &i : valid_hostnames)
    REQUIRE(utils::validator::valid_hostname(i));

  std::vector<std::string> invalid_hostnames{};
  invalid_hostnames.reserve(10);
  std::string temp{};
  for (int i = 0; i < 10; ++i) {
    utils::randomizer::randomstr(temp);
    invalid_hostnames.emplace_back(temp);
  }

  for (auto &i : invalid_hostnames)
    REQUIRE_FALSE(utils::validator::valid_hostname(i));
}

TEST_CASE("test utils valid port") {
  std::string temp{};
  int i;
  for (i = 1; i < 65535; ++i) {
    temp = std::to_string(i);
    REQUIRE(utils::validator::valid_port(temp));
  }

  for (i = 0; i > -65536; --i) {
    temp = std::to_string(i);
    REQUIRE_FALSE(utils::validator::valid_port(temp));
  }

  for (i = 65535; i < 131070; ++i) {
    temp = std::to_string(i);
    REQUIRE_FALSE(utils::validator::valid_port(temp));
  }
}
