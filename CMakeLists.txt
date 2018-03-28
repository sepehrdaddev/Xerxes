cmake_minimum_required(VERSION 3.9)

project(Xerxes)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
endif()

add_executable(Xerxes main.cpp Configuration.h Doser.cpp Doser.h Validator.cpp Validator.h Parser.cpp Parser.h Logger.cpp Logger.h headers.h)
target_link_libraries(Xerxes ${OPENSSL_LIBRARIES})
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/useragents ${CMAKE_CURRENT_BINARY_DIR}/useragents)