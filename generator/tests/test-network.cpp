#include "../headers/network.h"
using ai::Network;
using ai::setupNetworks;

#include "catch.hpp"

#include <vector>
using std::vector;

TEST_CASE("Test the network class") {
    SECTION("testing setup of networks") {
        vector<unsigned int> dimesions{4, 32, 40, 10, 1};
        setupNetworks(dimesions, 2);
    }
}
