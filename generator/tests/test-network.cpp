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
        Network(0);
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
    SECTION("Testing space and default vec are equal") {
        REQUIRE(emptyBoard[0] == ' ');
    }
    SECTION("test network evaluation works.") {
        std::cout << "Board was evaluated as: " << player.evaluateBoard(emptyBoard) << std::endl;
    }
    SECTION ("Ensure output of a big board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleBigBoard) == player.evaluateBoard(sampleBigBoard));
    }
    SECTION ("Ensure output of a small board evaluation is consistent.") {
        REQUIRE(player.evaluateBoard(sampleSmallBoard) == player.evaluateBoard(sampleSmallBoard));
    }
}
