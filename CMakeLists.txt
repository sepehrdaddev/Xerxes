cmake_minimum_required(VERSION 3.9)

project(Xerxes)

set(CMAKE_CXX_STANDARD 11)

add_executable(Xerxes main.cpp Configuration.h Doser.cpp Doser.h Validator.cpp Validator.h Parser.cpp Parser.h Logger.cpp Logger.h)
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/useragents ${CMAKE_CURRENT_BINARY_DIR}/useragents)