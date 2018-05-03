#include "move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "json-to-stl.h"
using ai::JsonToStlConverter;
using ai::loadMoveTableFrom;

#include "models.h"
using ai::Jump;
using ai::Piece;
using ai::Position;

#include "table-types.h"
using ai::JumpTableType;
using ai::MoveTableType;

#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <utility>
using std::make_pair;
using std::pair;
#include <stdio.h>

MoveGenerator::MoveGenerator(const MoveTableType &moves,
                             const JumpTableType &jumps)
    : moves(moves), jumps(jumps) {}
// MoveGenerator::~MoveGenerator(){};

vector<int> MoveGenerator::getMoves(int space) const {
  return this->moves[space];
}

vector<Jump> MoveGenerator::getJumps(int space) const {
  return this->jumps[space];
}

MoveGenerator ai::getGeneratorFor(const string &color,
                                  const JsonToStlConverter &converter) {
  auto moves = converter.getMovesFor(color);
  auto jumps = converter.getJumpsFor(color);

  return MoveGenerator(moves, jumps);
}

MoveGenerator ai::getKingGenerator(const JsonToStlConverter &converter) {
  auto redMoves = converter.getMovesFor("red");
  auto blackMoves = converter.getMovesFor("black");

  MoveTableType moves;
  for (auto i = 0; i < static_cast<int>(redMoves.size()); ++i) {
    auto r = redMoves[i];
    auto b = blackMoves[i];

    r.insert(r.end(), b.begin(), b.end());

    moves.push_back(r);
  }

  auto redJumps = converter.getJumpsFor("red");
  auto blackJumps = converter.getJumpsFor("black");

  JumpTableType jumps;
  for (auto i = 0; i < static_cast<int>(redMoves.size()); ++i) {
    auto r = redJumps[i];
    auto b = blackJumps[i];

    r.insert(r.end(), b.begin(), b.end());

    jumps.push_back(r);
  }

  return MoveGenerator(moves, jumps);
}
