#include "headers/json-to-stl.h"

#include <iostream>
using std::cout;
using std::endl;

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

