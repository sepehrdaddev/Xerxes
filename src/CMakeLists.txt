set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(../lib/args)
include_directories(../lib/libressl)

add_executable(Xerxes main.cpp engine.cpp engine.h utils.h config.h tcp_flood.cpp tcp_flood.h socket.cpp socket.h ssocket.cpp ssocket.h)

target_link_libraries(Xerxes tls ssl crypto pthread ${CMAKE_DL_LIBS})
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/useragents ${CMAKE_BINARY_DIR}/bin/useragents)
add_custom_command(TARGET Xerxes POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/Dockerfile ${CMAKE_BINARY_DIR}/bin/Dockerfile)
