#include "headers/network.h"
using ai::setupNetworks;
using ai::Network;
#include "headers/consts.h"
using ai::NETWORK_DIMENSIONS;
using ai::NETWORKPOPSIZE;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <experimental/filesystem>
using std::experimental::filesystem::create_directories;
using std::experimental::filesystem::remove;
#include <string>
using std::string;
// #include <sstream>
// using std::istringstream;
#include "headers/utils.h"
using ai::getUsername;
#include <vector>
using std::vector;
#include <sstream>
using std::stringstream;
// #include <string.h>

int main(int argc, char *argv[]) {
    const auto SETUP_NETWORKS = '0';
    const auto STORE_PERFORMANCES = '1';
    const auto EVOLVE_NETWORKS = '2';

    if (argc == 1) {
        cout << "You called me without any input" << endl;
        return -1;
    }
    string username(getUsername());
    if (*argv[1] == SETUP_NETWORKS) {
        string path = "./networks/" + username;

        cout << "You want me to setup networks." << endl;
        cout << "the path I created for your directory was: " << path << endl;
        create_directories(path) ? cout << "Your directory was created" << endl : 
                                cout << "Couldn't create directory (is it already created?)" << endl;

        setupNetworks(NETWORK_DIMENSIONS, NETWORKPOPSIZE);
    }
    else if (*argv[1] == STORE_PERFORMANCES) {
        cout << "You want me to store performances." << endl;

        vector<int> performances;
        string input(argv[2]);
        stringstream iss (input);

        int temp;
        while(iss >> temp){
            performances.push_back(temp);
        }

        //debug
        for (auto i: performances) {
            cout << i << endl;
        }

        if (performances.size() != NETWORKPOPSIZE) {
            cout << "*** Incorrect Number of Weights! ***" << endl;
            return -1;
        }
        
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            Network netPerformanceAdjust(i);
            netPerformanceAdjust.adjustPerformance(performances[i]);
            ai::Network::save(i, netPerformanceAdjust);
        }
        cout << "---- Performances reloaded ----" << endl;
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            Network getPer(i);
            cout << getPer.getPerformance() << endl;
        }
    }
    else if (*argv[1] == EVOLVE_NETWORKS) {
        cout << "You want me to evolve your networks" << endl;
    }
    return 0;
}
