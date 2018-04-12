cmake_minimum_required(VERSION 2.8.9)
project(CheckersAi)

# Compiler Options
set(CMAKE_CXX_FLAGS "-O3 -Wall")
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(CheckersMain INTERFACE)

set(SRC_DIR "src")

set(HEADERS_DIR "headers")
include_directories(${HEADERS_DIR})

#Can manually add the sources using the set command as follows:
set(SOURCES
    ${SRC_DIR}/move-generator.cpp
    ${SRC_DIR}/json-to-stl.cpp
    ${SRC_DIR}/checkers-game.cpp
    ${SRC_DIR}/player.cpp
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/board.cpp
    ${SRC_DIR}/seeder.cpp
    ${SRC_DIR}/network.cpp
    ${SRC_DIR}/network-file-reader.cpp
    ${SRC_DIR}/network-file-writer.cpp
    ${SRC_DIR}/search.cpp
    ${SRC_DIR}/game-state.cpp
)

set(HEADERS
    ${HEADERS_DIR}/move-generator.h
    ${HEADERS_DIR}/json-to-stl.h
    ${HEADERS_DIR}/checkers-game.h
    ${HEADERS_DIR}/player.h
    ${HEADERS_DIR}/utils.h
    ${HEADERS_DIR}/board.h
    ${HEADERS_DIR}/seeder.h
    ${HEADERS_DIR}/network.h
    ${HEADERS_DIR}/network-file-io.h
    ${HEADERS_DIR}/search.h
    ${HEADERS_DIR}/game-state.h
)

set(SKYNET_INCLUDE_DIR ../skynet/)
add_library(Skynet INTERFACE)
target_include_directories(Skynet INTERFACE ${SKYNET_INCLUDE_DIR})
add_executable(checkers.out
    ${HEADERS}
    ${SRC_DIR}/checkers_skynet_interface.cpp
    ${HEADERS_DIR}/checkers_skynet_interface.h
    ${SRC_DIR}/main.cpp
    ${SOURCES}
)

add_executable(turn.out
    ${HEADERS}
    ${SRC_DIR}/turn-main.cpp
    ${SOURCES}
)

target_link_libraries(checkers.out Skynet)

set(MANAGER_SOURCES
    ${SRC_DIR}/networkManagement.cpp
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/network.cpp
    ${SRC_DIR}/seeder.cpp
    ${SRC_DIR}/network-file-reader.cpp
    ${SRC_DIR}/network-file-writer.cpp
)
add_library(stdc++fs UNKNOWN IMPORTED)

include(libstdcppfsPath.txt)
# this file looks like
#
#       set(LIBSTDCPPFS_PATH "/usr/lib/gcc/x86_64-linux-gnu/5/libstdc++fs.a")
#                                  (Your path may vary...)

set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION ${LIBSTDCPPFS_PATH})

add_executable(manager.out ${MANAGER_SOURCES})
target_link_libraries(manager.out stdc++fs)

set(CATCH_INCLUDE_DIR tests)
set(TEST_SOURCES
    tests/test-main.cpp
    tests/test-json-to-stl.cpp
    tests/test-player.cpp
    tests/test-utils.cpp
    tests/test-move-generator.cpp
    tests/test-network.cpp
    tests/test-search.cpp
)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
add_executable(test.out ${TEST_SOURCES} ${SOURCES})
target_link_libraries(test.out Catch)




enable_testing()
add_test(NAME MyAwesomeTest COMMAND test.out)
