#include "json.hpp"
using json = nlohmann::json;

#include "table-types.h"
#include "jump.h"

#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
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

class JsonToStlConverter {
    private:
        json moveTable;

    public:
        JsonToStlConverter(json table): moveTable(table) {
        }

        MoveTableType getMovesFor(const string & color) {
            auto movesJson = this->moveTable[color]["moves"];
            auto moveTableStl = movesToStlContainer(movesJson);

            return moveTableStl;
        }

        JumpTableType getJumpsFor(const string & color) {
            auto jumpsJson = this->moveTable[color]["jumps"];
            auto jumpTableStl = jumpsToStlContainer(jumpsJson);

            return jumpTableStl;
        }

    private:
        MoveTableType movesToStlContainer(const json & movesJson) {
            MoveTableType moveTableStl;

            auto end = movesJson.end();
            for (auto it = movesJson.begin(); it != end; ++it) {

                auto moves = getMovesFromJson(it.value());
                auto boardSpace = strToInt(it.key());

                moveTableStl[boardSpace] = moves;
            }

            return moveTableStl;
        }

        JumpTableType jumpsToStlContainer(const json & jumpsJson) {
            JumpTableType jumpTableStl;

            auto end = jumpsJson.end();
            for (auto it = jumpsJson.begin(); it != end; ++it) {

                auto jumps = getJumpsFromJson(it.value());
                auto boardSpace = strToInt(it.key());

                jumpTableStl[boardSpace] = jumps;
            }

            return jumpTableStl;
        }

        vector<Jump> getJumpsFromJson(const json & jumpsForSpace) {
            vector<Jump> jumps;

            for (auto & j: jumpsForSpace) {
                auto jump = Jump(j[0], j[1]);
                jumps.push_back(jump);
            }

            return jumps;
        }

        vector<int> getMovesFromJson(const json & movesForSpace) {
            vector<int> moves;

            for (auto & move: movesForSpace) {
                moves.push_back(move);
            }

            return moves;
        }

        int strToInt(const string & s) {
            stringstream ss(s);
            int key = 0;
            ss >> key;

            return key;
        }
};

auto testJson() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto redMoves = converter.getMovesFor("red");
    auto blackMoves = converter.getMovesFor("black");
    auto redJumps = converter.getJumpsFor("red");

    cout << redJumps[0][0].to << endl;
    cout << redJumps[0][0].from << endl;
}

int main() {
    testJson();
    return 0;
}

