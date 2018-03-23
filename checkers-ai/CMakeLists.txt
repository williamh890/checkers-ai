cmake_minimum_required(VERSION 2.8.9)
project(CheckersAi)

# Compiler Options
set(CMAKE_CXX_FLAGS "-O3 -Wall")
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Can manually add the sources using the set command as follows:
set(SOURCES
    move-generator.cpp
    json-to-stl.cpp
    checkers-game.cpp
    player.cpp
    utils.cpp
    board.cpp
    seeder.cpp
    network.cpp
    network-file-reader.cpp
    network-file-writer.cpp
    search.cpp
    game-state.cpp
)
add_executable(checkers.out main.cpp ${SOURCES})

set(MANAGER_SOURCES
    networkManagement.cpp
    utils.cpp
    network.cpp
    seeder.cpp
    network-file-reader.cpp
    network-file-writer.cpp
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
