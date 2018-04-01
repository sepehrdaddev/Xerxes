cmake_minimum_required(VERSION 3.9)

project(Xerxes)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif()

add_executable(Xerxes main.cpp Configuration.hpp Doser.cpp Doser.hpp Validator.cpp Validator.hpp Parser.cpp Parser.hpp Logger.cpp Logger.hpp ICMP_Flood.cpp ICMP_Flood.hpp Attack_Vectors.hpp Randomizer.hpp Null_Flood.cpp Null_Flood.hpp Slowloris.cpp Slowloris.hpp Http_Flood.cpp Http_Flood.hpp Spoofed_Flood.cpp Spoofed_Flood.hpp Spoofed_TCP_Flood.cpp Spoofed_TCP_Flood.hpp Spoofed_UDP_Flood.cpp Spoofed_UDP_Flood.hpp)
target_link_libraries(Xerxes ${OPENSSL_LIBRARIES})
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/useragents ${CMAKE_CURRENT_BINARY_DIR}/useragents)