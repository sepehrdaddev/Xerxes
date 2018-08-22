set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
endif()
add_executable(Xerxes Sources/main.cpp Headers/Configuration.hpp Sources/Engine.cpp Headers/Engine.hpp Sources/Validator.cpp Headers/Validator.hpp 
					 	Sources/Parser.cpp Headers/Parser.hpp Sources/ICMP_Flood.cpp Headers/ICMP_Flood.hpp Headers/Attack_Vectors.hpp Headers/Randomizer.hpp 
					 	Sources/Null_Flood.cpp Headers/Null_Flood.hpp Sources/Slowloris.cpp Headers/Slowloris.hpp Sources/Http_Flood.cpp Headers/Http_Flood.hpp 
					 	Sources/Spoofed_Flood.cpp Headers/Spoofed_Flood.hpp Sources/Spoofed_TCP_Flood.cpp Headers/Spoofed_TCP_Flood.hpp Sources/Spoofed_UDP_Flood.cpp 
					 	Headers/Spoofed_UDP_Flood.hpp Sources/Attack_Vector.cpp Headers/Attack_Vector.hpp Headers/Version.hpp Sources/Beast.cpp Headers/Beast.hpp 
					 	Sources/httphdr.cpp Headers/httphdr.hpp Headers/Logging.hpp)
target_link_libraries(Xerxes ${OPENSSL_LIBRARIES} pthread ${CMAKE_DL_LIBS})
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/useragents ${CMAKE_BINARY_DIR}/bin/useragents)
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/Dockerfile ${CMAKE_BINARY_DIR}/bin/Dockerfile)
