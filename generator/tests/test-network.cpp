#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "catch.hpp"

#include <vector>
using std::vector;

TEST_CASE("Test the network class") {
    vector<unsigned int> dimesions{4, 32, 40, 10, 1};
    setupNetworks(dimesions, 2);

    SECTION("test loading a network") {
        Network(0);
    }

    SECTION("test network evaluation") {
        vector<char> sampleBoard{ 'r','r','r','r','r','r','r','r',' ','r','r','r',' ', ' ','r', ' ',' ', ' ', 'b', ' ','b','b',' ','b','b','b','b','b','b','b','b','b' };

        ai::Network player(0);
        //Proof of success at runtime
        //player.outputDebug();
        std::cout << "Board was evaluated as: " << player.evaluateBoard(sampleBoard) << std::endl;
    }
}
