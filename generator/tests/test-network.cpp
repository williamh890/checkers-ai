#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "catch.hpp"

#include <vector>
using std::vector;

TEST_CASE("Test the network class") {
    vector<unsigned int> dimesions{4, 32, 40, 10, 1};
    //setupNetworks(dimesions, 2);

    SECTION("test loading a network") {
       // Network(0);
    }
}
TEST_CASE("Test saving and loading consistency") {
    SECTION ("Testing loading consistency") {
        ai::Network player(0);
        ai::Network playerAgain(0);
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

    SECTION("test network evaluation works.") {
        REQUIRE(player.evaluateBoard(emptyBoard));
    }
    SECTION ("Ensure output of a big board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleBigBoard) == player.evaluateBoard(sampleBigBoard));
    }
    SECTION ("Ensure output of a small board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleSmallBoard) == player.evaluateBoard(sampleSmallBoard));
    }
}
