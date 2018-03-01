//#include "headers/network.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

int main(int argc, char *argv[]) {
    int returnVal = 0;
    if (argc == 1) {
        cout << "You called me without any input" << endl;
        returnVal = -1;
    }

    else if (*argv[1] == '0') {
        cout << "You want me to setup networks." << endl;
    }
    else if (*argv[1] == '1') {
        cout << "You want me to store performances." << endl;
    }
    else if (*argv[1] == '2') {
        cout << "You want me to evolve your networks" << endl;
    }
    return returnVal;
}
