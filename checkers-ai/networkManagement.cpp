#include "headers/network.h"
using ai::setupNetworks;
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
        cout << "You want me to setup networks." << endl;
        string path = "./networks/" + username;
        cout << "the path I created for your directory was: " << path << endl;
        create_directories(path) ? cout << "Your directory was created" << endl : 
                                cout << "Couldn't create directory (is it already created?)" << endl;
        //remove(path) ? cout << "I removed it to clean up" << endl : 
        //                cout << "Couldn't remove the directory" << endl;
        vector<unsigned int> blondieDimensions{32, 40, 10, 1};
        int blondiePopulation = 30;
        setupNetworks(blondieDimensions, blondiePopulation);
    }
    else if (*argv[1] == STORE_PERFORMANCES) {
        cout << "You want me to store performances." << endl;
    }
    else if (*argv[1] == EVOLVE_NETWORKS) {
        cout << "You want me to evolve your networks" << endl;
    }
    return 0;
}
