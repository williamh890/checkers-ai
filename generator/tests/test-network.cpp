#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "../headers/network-file-io.h"
using ai::loadNetwork;
using ai::saveNetwork;
#include "catch.hpp"

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

TEST_CASE("Test the network class") {
    vector<unsigned int> dimesions{4, 32, 40, 10, 1};
    //setupNetworks(dimesions, 2);
}
TEST_CASE("Test saving and loading consistency") {
    ai::Network player(0);
    ai::Network playerAgain(0);
    SECTION ("Testing loading consistency") {
        REQUIRE(player == playerAgain);
    }
    SECTION ("Loading a saved object == object that was saved.") {
    saveNetwork(0, player);
    loadNetwork(0, playerAgain);
    REQUIRE(player == playerAgain);
    }
}
TEST_CASE("Test Network Evaluation") {
    ai::Network player(0);
    vector<char> emptyBoard(32);
    vector<char> sampleBigBoard{
            'r',   'r',   'r',   'r',
         'r',   'r',   'r',   'r',
            ' ',   'r',   'r',   'r',
         ' ',   ' ',   'r',   ' ',
            ' ',   ' ',   'b',   ' ',
         'b',   'b',   ' ',   'b',
            'b',   'b',   'b',   'b',
         'b',   'b',   'b',   'b'
        };
        vector<char> sampleSmallBoard(32);
        sampleSmallBoard[0] = 'r';

    SECTION("test network evaluation of empty board is 0.") {
        REQUIRE(player.evaluateBoard(emptyBoard) == 0);
    }
    SECTION ("Ensure output of a big board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleBigBoard) == player.evaluateBoard(sampleBigBoard));
    }
    SECTION ("Ensure output of a small board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleSmallBoard) == player.evaluateBoard(sampleSmallBoard));
    }
}
