#include "headers/move-generator.h"
using ai::MoveGenerator;
using ai::getGeneratorFor;

#include "headers/checkers-game.h"
using ai::CheckersGame;
using ai::getCheckersGame;

#include "headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "headers/models.h"
using ai::Position;
using ai::Piece;
using ai::Jump;

#include "headers/table-types.h"
using ai::MoveTableType;
using ai::JumpTableType;

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <utility>
using std::pair;
using std::make_pair;
#include <stdio.h>

MoveGenerator::MoveGenerator(){};
MoveGenerator::MoveGenerator(
        const MoveTableType & moves,
        const JumpTableType & jumps): moves(moves), jumps(jumps) {
}
// MoveGenerator::~MoveGenerator(){};

vector<int> MoveGenerator::getMoves(int space) {
    return this->moves[ space ];
}

vector<Jump> MoveGenerator::getJumps(int space) {
    return this->jumps[ space ];
}

MoveGenerator ai::getGeneratorFor(const string & color, const JsonToStlConverter & converter) {
    auto moves = converter.getMovesFor(color);
    auto jumps = converter.getJumpsFor(color);

    return MoveGenerator(moves, jumps);
}
