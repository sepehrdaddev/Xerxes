#define CATCH_CONFIG_MAIN

#include "../../lib/catch/catch.hpp"
#include "../../src/utils.h"

#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

TEST_CASE("test utils random int"){
    std::vector<int> nums{};
    nums.reserve(10000);
    int min = -100000, max = 100000, rand;
    for(int i = 0; i < 10000; ++i){
        rand = utils::randomizer::randomInt(min, max);
        REQUIRE(rand >= min);
        REQUIRE(rand <= max);
        nums.emplace_back(rand);
    }
    REQUIRE(std::adjacent_find(nums.begin(), nums.end()) == nums.end());
}

TEST_CASE("test utils random ip"){
    std::vector<std::string> ips{};
    ips.reserve(10000);
    std::string temp{};
    std::vector<std::string> temp_vec{};
    for(int i = 0; i < 10000; ++i) {
        utils::randomizer::randomIP(temp);
        temp_vec = split(temp, '.');
        REQUIRE(temp_vec.size() == 4);
        for(auto& val : temp_vec){
            REQUIRE(utils::to_int(val) > 0);
            REQUIRE(utils::to_int(val) < 256);
        }

        ips.emplace_back(temp);
    }

    REQUIRE(std::adjacent_find(ips.begin(), ips.end()) == ips.end());
}

TEST_CASE("test utils random port"){
    std::vector<int> ports{};
    ports.reserve(10000);
    int port;
    bool is_continious = true;
    for(int i = 0; i < 10000; ++i){
        port = utils::randomizer::randomPort();
        REQUIRE(port > 0);
        REQUIRE(port < 65536);
        ports.emplace_back(port);
    }

    for(unsigned long i = 0; i < ports.size(); ++i){
        if((i+1 != ports.size()) && (ports[i] + 1 != ports[i+1])){
            is_continious = false;
        }
    }

    REQUIRE(std::adjacent_find(ports.begin(), ports.end()) == ports.end());
    REQUIRE(is_continious);
}

TEST_CASE("test utils random str"){
    std::vector<std::string> strs{};
    strs.reserve(10000);
    std::string temp{};
    for(int i = 0; i < 10000; ++i) {
        utils::randomizer::randomstr(temp);
        REQUIRE_FALSE(temp.empty());
        REQUIRE(temp.size() <= 30);
        strs.emplace_back(temp);
    }

    REQUIRE(std::adjacent_find(strs.begin(), strs.end()) == strs.end());
}

TEST_CASE("test utils set dly"){
    timespec time{};
    int dly{};

    for(int i = 1; i < 5000; ++i){
        dly = 999999999 + i;
        utils::set_dly(dly, &time);
        REQUIRE(time.tv_sec == i);
        REQUIRE(time.tv_nsec == (dly - 1000000000));
    }

    for(int i = 0; i < 5000; ++i){
        dly = 999999999 - i;
        utils::set_dly(dly, &time);
        REQUIRE(time.tv_sec == 0);
        REQUIRE(time.tv_nsec == dly);
    }

    dly = 0;
    utils::set_dly(dly, &time);
    REQUIRE(time.tv_sec == 0);
    REQUIRE(time.tv_nsec == 1);


}

TEST_CASE("test utils to_int"){
    for(int i = -10000; i < 10000; ++i){
        REQUIRE(utils::to_int(std::to_string(i)) == i);
    }
}

TEST_CASE("test utils valid port"){
    std::string temp{};
    int i;
    for(i = 1; i < 65535; ++i){
        temp = std::to_string(i);
        REQUIRE(utils::validator::valid_port(temp));
    }

    for(i = 0; i > -65536; --i){
        temp = std::to_string(i);
        REQUIRE_FALSE(utils::validator::valid_port(temp));
    }

    for(i = 65535; i < 131070; ++i){
        temp = std::to_string(i);
        REQUIRE_FALSE(utils::validator::valid_port(temp));
    }

}