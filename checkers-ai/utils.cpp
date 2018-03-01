#include "headers/utils.h"

#include "headers/models.h"
using ai::Position;

#include <string>
using std::string;
using std::to_string;

#include <chrono>

#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::ios;

/*         0   1   2   3   4   5   6   7
         +---+---+---+---+---+---+---+---+
     0   | X | 0 |   | 1 |   | 2 |   | 3 |
         +---+---+---+---+---+---+---+---+
     1   | 4 |   | 5 |   | 6 |   | 7 |   |
         +---+---+---+---+---+---+---+---+
     2   |   | 8 |   | 9 |   | 10|   | 11|
         +---+---+---+---+---+---+---+---+
     3   | 12|   | 13|   | 14|   | 15|   |
         +---+---+---+---+---+---+---+---+
     4   |   | 16|   | 17|   | 18|   | 19|
         +---+---+---+---+---+---+---+---+
     5   | 20|   | 21|   | 22|   | 23|   |
         +---+---+---+---+---+---+---+---+
     6   |   | 24|   | 25|   | 26|   | 27|
         +---+---+---+---+---+---+---+---+
     7   | 28|   | 29|   | 30|   | 31| X |
         +---+---+---+---+---+---+---+---+
*/

Position ai::spaceToPosition(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2 == 1) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}


int ai::positionToSpace(const Position & pos) {
    int space = 0;

    space += pos.row * 4;
    space += (pos.col / 2);

    return space;
}

string ai::getUsername () {
    ifstream usernameFile;
    usernameFile.open("user.me", ios::out | ios::binary);
    if(!usernameFile) {
        ofstream placingUsernameFile;
        placingUsernameFile.open("user.me", ios::out | ios::binary);
        cout << "Give me your username: ";
        char inputName[25];
        std::cin.getline(inputName, 25);
        placingUsernameFile << inputName;
        placingUsernameFile.close();
        usernameFile.clear();
        usernameFile.open("user.me", ios::out | ios::binary);
    }
    string nameOut;
    usernameFile >> nameOut;
    cout << "You're name is: " << nameOut << endl;
    return nameOut;
}

string ai::idToFilename(int ID) {
	//string filename = to_string(ID) + ".network";
	// The following implementation will be used once we begin to get the network integrated.
	string filename = "./networks/" + getUsername() + "/" + to_string(ID) + ".network"; //creates filenames that scope to a folder called networks
	return filename;
}

double ai::getTime() {
    return 1.0e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count();
}

