#define CATCH_CONFIG_MAIN

#include "../../lib/catch/catch.hpp"
#include "../../src/utils.h"

#include <iostream>
#include <vector>

TEST_CASE("test utils random int"){
    std::vector<int> nums{};
    nums.reserve(10000);
    for(int i = 0; i < 10000; ++i)
        nums.emplace_back(utils::randomInt(0, INT32_MAX));
    REQUIRE(std::adjacent_find(nums.begin(), nums.end()) == nums.end());
}

TEST_CASE("test utils random ip"){
    std::vector<std::string> ips{};
    ips.reserve(10000);
    std::string temp{};
    for(int i = 0; i < 10000; ++i) {
        utils::randomIP(temp);
        ips.emplace_back(temp);
    }

    REQUIRE(std::adjacent_find(ips.begin(), ips.end()) == ips.end());
}

TEST_CASE("test utils random port"){
    std::vector<int> ports{};
    ports.reserve(10000);
    bool is_continious = true;
    for(int i = 0; i < 10000; ++i)
        ports.emplace_back(utils::randomPort());

    for(unsigned long i = 0; i < ports.size(); ++i){
        if((i+1 != ports.size()) && (ports[i] + 1 != ports[i+1])){
            printf("%d %d", ports[i], ports[i+1]);
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
        utils::randomstr(temp);
        strs.emplace_back(temp);
    }

    REQUIRE(std::adjacent_find(strs.begin(), strs.end()) == strs.end());
}