// run this to setup 100 new networks
#include "headers/network.h"
#include <iostream>
#include <chrono>
#include <vector>
using std::vector;

int main() {
    vector <unsigned int> networkCreationInput = {32, 40, 10, 1 };

    ai::setupNetworks(networkCreationInput, 100);

    // Wait for user
    std::cin.ignore();
    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n');
    return 0;
}
