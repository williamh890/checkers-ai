//#include "headers/network.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <experimental/filesystem>
using std::experimental::filesystem::create_directory;
using std::experimental::filesystem::remove;
#include <string>
using std::string;
// #include <sstream>
// using std::istringstream;

int main(int argc, char *argv[]) {
    int returnVal = 0;
    if (argc == 1) {
        cout << "You called me without any input" << endl;
        returnVal = -1;
    }

    else if (*argv[1] == '0') {
        cout << "You want me to setup networks." << endl;
        if (argc <3) {
            cout << "You didn't give me a username. " << endl;
            returnVal = -1;
        }
        else {
            string username(argv[2]);
            string path = "./networks/" + username;
            cout << "the path I created for your directory was: " << path << endl;
            create_directory(path) ? cout << "Your directory was created" << endl : 
                                    (cout << "Couldn't create directory" << endl);
            //remove(path) ? (cout << "I removed it to clean up" << endl) : 
            //                (cout << "Couldn't remove the directory" << endl);
        }
    }
    else if (*argv[1] == '1') {
        cout << "You want me to store performances." << endl;
    }
    else if (*argv[1] == '2') {
        cout << "You want me to evolve your networks" << endl;
    }
    return returnVal;
}
