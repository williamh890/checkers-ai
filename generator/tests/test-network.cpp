#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "catch.hpp"

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

TEST_CASE("Test the network class") {
    SECTION("Test setupNetworks") {
        vector<unsigned int> dimesions{4, 32, 40, 10, 1};
        setupNetworks(dimesions, 2);
    }

    SECTION("test loading a network") {
        Network(0);
    }

    SECTION("test network evaluation") {
        vector<char> sampleBoard{
            'r',   'r',   'r',   'r',
         'r',   'r',   'r',   'r',
            ' ',   'r',   'r',   'r',
         ' ',   ' ',   'r',   ' ',
            ' ',   ' ',   'b',   ' ',
         'b',   'b',   ' ',   'b',
            'b',   'b',   'b',   'b',
         'b',   'b',   'b',   'b'
        };

        ai::Network player(0);
        //Proof of success at runtime
        //player.outputDebug();
        cout << "Board was evaluated as: " << player.evaluateBoard(sampleBoard) << endl;
    }
}
