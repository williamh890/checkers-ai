#include "network.h"
using ai::setupNetworks;
using ai::Network;
#include "consts.h"
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
#include "utils.h"
using ai::getUsername;
using ai::NetworkTestingNamechange;
#include <vector>
using std::vector;
#include <sstream>
using std::stringstream;
#include <iterator>
using std::istream_iterator;
#include <algorithm>
using std::sort;

int main(int argc, char *argv[]) {
    const auto SETUP_NETWORKS = '0';
    const auto STORE_PERFORMANCES = '1';
    const auto EVOLVE_NETWORKS_ALL = '2';
    const auto EVOLVE_USER_NETWORKS = '3';

    //cout << "Username: " << getUsername() << endl;

    if (argc == 1) {
        cout << "manager.out [0|1|2|3]" << endl
             << "  0 -> Setup Networks" << endl
             << "  1 -> Store Performances" << endl
             << "  2 -> Evolve All Networks" << endl
             << "  3 -> Evolve User Networks" << endl
             << endl;
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
        //cout << "You want me to store performances." << endl;

        if (argv[2] == nullptr) {
            cout << "You didn't give me performances!" << endl;
            return -1;
        }

        vector<int> performances;
        string input(argv[2]);
        //cout<<"input was "<<input<<endl;
        stringstream iss (input);

        int temp;
        while(iss >> temp){
            performances.push_back(temp);
            if (iss.peek() == ','){
              iss.ignore();
            }
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
        // cout << "List of directories: " << endl;
        // for (auto i: folderPaths) {
        //     cout << i << endl;
        // }

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
        // cout << "*** All paths stored in networkPathsContainer" << endl;
        // for (auto i : networkPathsContainer) {
        //     for (auto j : i)
        //         cout << j << endl;
        // }

        ////////////////////////////////////////////////////////////////
        // Begin process of sorting all networks for stealing
        /////////////////////////////////
        string myFilePath = "./networks/" + getUsername() + "/0.network";
        cout << "Username: " << getUsername() << endl;
        vector<Network> myNetworks;
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            myNetworks.push_back(Network(i));
        }
        cout << "sorting for myNetworks is off" << endl;
        //sort(myNetworks.begin(), myNetworks.end(), [](Network first, Network second) {return first > second;});

        // for (auto i : myNetworks) {
        //     cout << i.getPerformance() << endl;
        // }


        // Begin getting other networks
        std::vector<vector<Network>> networksContainer;
        for (auto i : networkPathsContainer) {
            if (i[0] == myFilePath) {
                cout << "Not opening my own networks here" << endl;
                continue;
            }
            vector<Network> networks_in_one_folder;

            /////////////////debug
            // cout << "Here's the paths i'm using: " << endl;
             for (auto j : i) {
                 Network temp;
            //     cout << j << endl;
                 Network::load(j, temp);
                 networks_in_one_folder.push_back(temp);
            }
            /////////////////////////////////////////

            /////////////////debug
            // cout << "*** This should be performances in Duane folder" << endl;
            // for (auto k : networks_in_one_folder) {
            //     cout << k.getPerformance() << endl;
            // }
            /////////////////////////////////////////

            sort(networks_in_one_folder.begin(), networks_in_one_folder.end(),
                    [](Network first, Network second) {return first > second;});

            ///////////////debug
            // cout << "*** This should be performances in all folders SORTED" << endl;
            // for (auto a : networks_in_one_folder) {
            //     cout << a.getPerformance() << endl;
            // }
            ///////////////////////////////////////
            networksContainer.push_back(networks_in_one_folder);
        }
        // TODO: Move best networks from other folders into myNetworks
        int leftovers = NETWORKPOPSIZE % networkPathsContainer.size();
        int numMyNetworksToKeep = leftovers + (NETWORKPOPSIZE/networkPathsContainer.size());
        unsigned int numNetworksToSteal = (NETWORKPOPSIZE-leftovers)/networkPathsContainer.size();
        cout << "Leftovers: " << leftovers << endl;
        cout << "Networks to keep: " << numMyNetworksToKeep << endl;
        cout << "Networks to steal: " << numNetworksToSteal << endl;
        auto startingPlaceToSave = numMyNetworksToKeep;
        for (auto i : networksContainer) {
            for (unsigned int j = 0; j < numNetworksToSteal; ++j) {
                cout << "For: " << startingPlaceToSave << " -- ";
                cout << "Taking: " << i[j].getPerformance();
                myNetworks[startingPlaceToSave] = std::move(i[j]);
                cout << " --- " << myNetworks[startingPlaceToSave].getPerformance() <<
                        " My networks should have same " << endl;
                ++startingPlaceToSave;
            }
        }
        for (auto i : myNetworks) {
            cout << i.getPerformance() << endl;
        }

        ////////////////////////////////////////////////////////////////
        // Evolve all of 'my' networks
        /////////////////////////////////
        cout << "evolving is off" << endl;
        // for (unsigned int i = 0; i < NETWORKPOPSIZE/2; ++i) {
        //     myNetworks[i+NETWORKPOPSIZE/2].evolveUsingNetwork(myNetworks[i]);
        //     Network::save(i, myNetworks[i]);
        //     Network::save(i+NETWORKPOPSIZE/2, myNetworks[i+NETWORKPOPSIZE/2]);
        //     //cout << "Done" << endl;
        // }
        // cout << "*** Performances should all be zero ***" << endl;
        // for (auto i : myNetworks) {
        //     cout << i.getPerformance() << endl;
        // }

    } // end evolve networks

    else if (*argv[1] == EVOLVE_USER_NETWORKS){
        //cout << "You want me to evolve just your networks" << endl;
        vector<Network> myNetworks;
        for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
            myNetworks.push_back(Network(i));
        }

        sort(myNetworks.begin(), myNetworks.end(), [](Network first, Network second) {return first > second;});

        // cout << "Sorted Performances: " << endl;
        // for (size_t i = 0; i < NETWORKPOPSIZE; ++i){
        //     cout<<myNetworks[i].getPerformance()<<" ";
        // }
        // cout << endl;

        int counter = 0;
        for (unsigned int i = 0; i < NETWORKPOPSIZE/2; ++i) {
            myNetworks[i+NETWORKPOPSIZE/2].evolveUsingNetwork(myNetworks[i]);
            if (myNetworks[i].getID() > 14)
                counter++;
            Network::save(i, myNetworks[i]);
            Network::save(i+NETWORKPOPSIZE/2, myNetworks[i+NETWORKPOPSIZE/2]);
            //cout << "Done" << endl;
        }
        // cout << "---- Performances reloaded ----" << endl;
        // for (unsigned int i = 0; i < NETWORKPOPSIZE; ++i) {
        //     Network getPer(i);
        //     cout << getPer.getPerformance() << " ";
        // }
        cout << "Number of children reproducing: " << counter;
        if (counter ==0 && myNetworks[0].getID() != 0) {
            cout << " new best network! ID: " << myNetworks[0].getID() << endl;
            for (unsigned int i = 0; i < myNetworks.size()/2; ++i) {
                cout << "i: " << i << " - " << myNetworks[i].getID() << endl;
            }
        }
    
    }

    else if (*argv[1] == '4') {
        ai::validateNetworks();
    }
    return 0;
}// end of main
