#include "json.hpp"
using json = nlohmann::json;

#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;
#include <sstream>
using std::stringstream;

json getMoveTableFromFile(const string & fileName) {
    ifstream jsonIn(fileName);
    json moveTable;
    jsonIn >> moveTable;

    return moveTable;
}

int strToInt(const string & s) {
    stringstream ss(s);
    int key = 0;
    ss >> key;

    return key;
}

vector<int> getMovesFromJson(const json & movesJson) {
    vector<int> moves;

    for (auto & move: movesJson) {
        moves.push_back(move);
    }

    return moves;
}

map<int, vector<int>> toStlContainer(const json & moveTableJson) {
    map<int, vector<int>> moveTable;

    auto end = moveTableJson.end();
    for (auto it = moveTableJson.begin(); it != end; ++it) {
        auto moves = getMovesFromJson(it.value());
        auto boardSpace = strToInt(it.key());

        moveTable[boardSpace] = moves;
    }

    return moveTable;
}

void testJson() {
    auto moveTableFilename = "move-table.json";
    auto moveTableJson = getMoveTableFromFile(moveTableFilename);

    auto redMoveJson = moveTableJson["red"]["moves"];
    auto redMoveTable = toStlContainer(redMoveJson);
    cout << redMoveTable[5][0];
}

int main() {
    testJson();
    return 0;
}

