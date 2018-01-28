#include "headers/table-types.h"
#include "headers/jump.h"
#include "headers/json-to-stl.h"

#include "headers/json.hpp"
using json = nlohmann::json;

#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;

json loadMoveTableFrom(const string & moveTableFilename) {
    ifstream jsonIn(moveTableFilename);
    json moveTableJson;
    jsonIn >> moveTableJson;

    return moveTableJson;
}

JsonToStlConverter::JsonToStlConverter(json table): moveTable(table) {
}

MoveTableType JsonToStlConverter::getMovesFor(const string & color) {
    auto movesJson = this->moveTable[color]["moves"];
    auto moveTableStl = movesToStlContainer(movesJson);

    return moveTableStl;
}

JumpTableType JsonToStlConverter::getJumpsFor(const string & color) {
    auto jumpsJson = this->moveTable[color]["jumps"];
    auto jumpTableStl = jumpsToStlContainer(jumpsJson);

    return jumpTableStl;
}

MoveTableType JsonToStlConverter::movesToStlContainer(const json & movesJson) {
    MoveTableType moveTableStl;

    auto end = movesJson.end();
    for (auto it = movesJson.begin(); it != end; ++it) {

        auto moves = getMovesFromJson(it.value());
        auto boardSpace = strToInt(it.key());

        moveTableStl[boardSpace] = moves;
    }

    return moveTableStl;
}

JumpTableType JsonToStlConverter::jumpsToStlContainer(const json & jumpsJson) {
    JumpTableType jumpTableStl;

    auto end = jumpsJson.end();
    for (auto it = jumpsJson.begin(); it != end; ++it) {

        auto jumps = getJumpsFromJson(it.value());
        auto boardSpace = strToInt(it.key());

        jumpTableStl[boardSpace] = jumps;
    }

    return jumpTableStl;
}

vector<Jump> JsonToStlConverter::getJumpsFromJson(const json & jumpsForSpace) {
    vector<Jump> jumps;

    for (auto & j: jumpsForSpace) {
        auto jump = Jump(j[0], j[1]);
        jumps.push_back(jump);
    }

    return jumps;
}

vector<int> JsonToStlConverter::getMovesFromJson(const json & movesForSpace) {
    vector<int> moves;

    for (auto & move: movesForSpace) {
        moves.push_back(move);
    }

    return moves;
}

int JsonToStlConverter::strToInt(const string & s) {
    stringstream ss(s);
    int key = 0;
    ss >> key;

    return key;
}
