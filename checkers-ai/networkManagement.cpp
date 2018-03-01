//#include "headers/network.h"
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

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cout << "You called me without any input" << endl;
        return -1;
    }
    string username(getUsername());
    if (*argv[1] == '0') {
        cout << "You want me to setup networks." << endl;
        if (argc <3) {
            cout << "You didn't give me a username. " << endl;
            return -1;
        }
        else {
            string path = "./networks/" + username;
            cout << "the path I created for your directory was: " << path << endl;
            create_directories(path) ? cout << "Your directory was created" << endl : 
                                    cout << "Couldn't create directory" << endl;
            //remove(path) ? cout << "I removed it to clean up" << endl : 
            //                cout << "Couldn't remove the directory" << endl;
            
        }
    }
    else if (*argv[1] == '1') {
        cout << "You want me to store performances." << endl;
    }
    else if (*argv[1] == '2') {
        cout << "You want me to evolve your networks" << endl;
    }
    return 0;
}
