cmake_minimum_required(VERSION 3.0)

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW) # CMake 3.0.0
endif()

project(Xerxes)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_TESTING OFF)

# LibreSSL Options
option(LIBRESSL_APPS OFF)
option(LIBRESSL_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)
option(BUILD_NC OFF)
option(LIBRESSL_SKIP_INSTALL ON)

# fmtlib Options
option(FMT_DOC OFF)
option(FMT_INSTALL OFF)
option(FMT_TEST OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

include_directories(include)
include_directories(lib/args)
include_directories(lib/fmt/include)
include_directories(lib/spdlog/include)
include_directories(lib/libressl/include)

add_subdirectory(lib/libressl)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/fmt)

file(GLOB SRCS src/*.cpp)
set(LIBS tls ssl crypto fmt pthread dl)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(Xerxes ${SRCS})
target_link_libraries(Xerxes ${LIBS})

include(PCH)
add_precompiled_header(Xerxes
                       include/stdafx.h
                       SOURCE_CXX
                       src/stdafx.cpp
                       FORCEINCLUDE)

file(COPY bash-completion DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY man DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY Dockerfile DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY useragents DESTINATION ${CMAKE_BINARY_DIR}/bin)
