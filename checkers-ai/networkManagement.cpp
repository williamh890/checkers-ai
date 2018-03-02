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
using std::experimental::filesystem::recursive_directory_iterator;
#include <string>
using std::string;
#include "headers/utils.h"
using ai::getUsername;
#include <vector>
using std::vector;
#include <sstream>
using std::stringstream;
#include <algorithm>
using std::sort;

int main(int argc, char *argv[]) {
    const auto SETUP_NETWORKS = '0';
    const auto STORE_PERFORMANCES = '1';
    const auto EVOLVE_NETWORKS_ALL = '2';
    const auto EVOLVE_USER_NETWORKS = '3';

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
        // for (auto i: performances) {
        //     cout << i << endl;
        // }

        if (performances.size() != NETWORKPOPSIZE) {
            cout << "*** Incorrect Number of Weights! ***" << endl;
            return -1;
        }
        
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            Network netPerformanceAdjust(i);
            netPerformanceAdjust.adjustPerformance(performances[i]);
            ai::Network::save(i, netPerformanceAdjust);
        }
        // cout << "---- Performances reloaded ----" << endl;
        // for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
        //     Network getPer(i);
        //     cout << getPer.getPerformance() << endl;
        // }
    }

    else if (*argv[1] == EVOLVE_NETWORKS_ALL) {
        cout << "You want me to evolve all networks" << endl;

        ////////////////////////////////////////////////////////////////
        // Get all current folders
        /////////////////////////////////
        const std::string s = "./networks";
        std::vector<std::string> folderPaths;
        for(auto& p : recursive_directory_iterator(s)){
            if(p.status().type() == std::experimental::filesystem::file_type::directory)
                folderPaths.push_back(p.path().string());
        }

        //debug
        cout << "List of directories: " << endl;
        for (auto i: folderPaths) {
            cout << i << endl;
        }

        cout << "There are: " << folderPaths.size() << " networks in development" << endl;
        
        ////////////////////////////////////////////////////////////////
        // Get all current networks
        /////////////////////////////////
        std::vector<vector<std::string>> networkPathsContainer;
        for (auto& folders : folderPaths) {
            vector<string> networkPaths;
            for(auto& p : recursive_directory_iterator(folders)){
                if(p.status().type() == std::experimental::filesystem::file_type::regular)
                    networkPaths.push_back(p.path().string());
            }
            networkPathsContainer.push_back(networkPaths);
        }
        for (auto i : networkPathsContainer) {
            for (auto j : i)
                cout << j << endl;
        }
        
        ////////////////////////////////////////////////////////////////
        // Begin process of sorting all networks for stealing
        /////////////////////////////////
        string myFilePath = "./networks/" + getUsername() + "/0.network";
        cout << "Username: " << getUsername() << endl;
        vector<Network> myNetworks;
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            myNetworks.push_back(Network(i));
        }
        sort(myNetworks.begin(), myNetworks.end(), [](Network first, Network second) {return first > second;});
        // for (auto i : myNetworks) {
        //     cout << i.getPerformance() << endl;
        // }

        std::vector<vector<Network>> networksContainer;
        for (auto i : networkPathsContainer) {
            if ( i[0] == myFilePath) {
                cout << "Not opening my own networks here" << endl;
                continue;
            }

        }
        // TODO: Take networks from other folders (sort them first)

        
        ////////////////////////////////////////////////////////////////
        // Evolve all of 'my' networks
        /////////////////////////////////
        for (unsigned int i = 0; i < NETWORKPOPSIZE/2; ++i) {
            myNetworks[i+NETWORKPOPSIZE/2].evolveUsingNetwork(myNetworks[i]);
            Network::save(i, myNetworks[i]);
            Network::save(i+NETWORKPOPSIZE/2, myNetworks[i+NETWORKPOPSIZE/2]);
            cout << "Done" << endl;
        }
        cout << "*** Performances should all be zero ***" << endl;
        for (auto i : myNetworks) {
            cout << i.getPerformance() << endl;
        }

    } // end evolve networks

    else if (*argv[1] == EVOLVE_USER_NETWORKS){
        cout << "You want me to evolve just your networks" << endl;
        vector<Network> myNetworks;
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            myNetworks.push_back(Network(i));
        }

        sort(myNetworks.begin(), myNetworks.end(), [](Network first, Network second) {return first > second;});

        for (unsigned int i = 0; i < NETWORKPOPSIZE/2; ++i) {
            myNetworks[i+NETWORKPOPSIZE/2].evolveUsingNetwork(myNetworks[i]);
            Network::save(i, myNetworks[i]);
            Network::save(i+NETWORKPOPSIZE/2, myNetworks[i+NETWORKPOPSIZE/2]);
            cout << "Done" << endl;
        }
    }
    return 0;
}// end of main
