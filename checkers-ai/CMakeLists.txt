
cmake_minimum_required(VERSION 2.8.9)
project(CheckersAi)

#Bring the headers, such as Student.h into the project
set(CMAKE_CXX_FLAGS "-O3 -Wall")

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
    main.cpp
)

add_executable(checkers.out ${SOURCES})

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

add_executable(my_test.out ${TEST_SOURCES})
target_link_libraries(my_test.out Catch)

enable_testing()
add_test(NAME MyAwesomeTest COMMAND my_test.out)

