#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;
using ai::PlayerType;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;

#include "headers/utils.h"
using ai::spaceToPosition;

#include "headers/table-types.h"
using ai::MoveTableType;

#include "headers/models.h"
using ai::Piece;
using ai::Jump;

#include "headers/consts.h"
using ai::TOTAL_NUM_PIECES;
using ai::INIT_NUM_PIECES;

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <sstream>
using std::stringstream;
#include <iostream>
using std::endl;
#include <utility>
using std::pair;


Player::Player(
        char color,
        MoveGenerator generator,
        PlayerType type=PlayerType::Computer) : color(color), generator(generator), playerType(type) {
}

void Player::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (isInitialSpace(space)) {
            Piece newPiece = Piece(color, space);
            pieces.push_back(newPiece);
        }
    }
}

const vector<Piece> Player::getPieces() const {
    return pieces;
}

const char Player::getColor() const {
    return color;
}

const PlayerType Player::getPlayerType() const {
    return playerType;
}

void Player::updatePieces(const pair<int, int> & move) {
    for (auto & piece : pieces) {
        if (piece.space == move.first) {
            piece.space = move.second;
            break;
        }
    }
}

vector<Jump> Player::getJumpsFor(const Piece & piece) const {
    return generator.getJumps( piece.space );
}

vector<int> Player::getMovesFor(const Piece & piece) const {
    return generator.getMoves( piece.space );
}

string Player::movesToString() const {
    stringstream ss;
    ss << color << " Moves: " << endl;

    for (const auto & checker : pieces) {
        auto s = spaceToPosition(checker.space);

        ss << "(" << s.row << ", " << s.col << "): ";
        for (const auto & move: getMovesFor(checker)) {
            ss << move << " ";
        }
        ss << "]"<<endl;
    }

    return ss.str();
}

string Player::jumpsToString() const {
    stringstream ss;
    ss << color << " Jumps: " << endl;

    for (const auto & checker: pieces) {
        ss << checker.space << ": ";

        for (const auto & jump: getJumpsFor(checker)) {
            ss << jump.to << ", " << jump.through << " ";
        }
        ss << endl;
    }

    return ss.str();
}


BlackPlayer::BlackPlayer(char color, MoveGenerator generator, PlayerType type=PlayerType::Human): Player(color, generator, type) {
    initPieces();
}

bool BlackPlayer::isInitialSpace(int space) const {
    return space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES);
}

RedPlayer::RedPlayer(char color,
                     MoveGenerator generator,
                     PlayerType type=PlayerType::Computer): Player(color, generator, type) {
    initPieces();
}

bool RedPlayer::isInitialSpace(int space) const {
    return space < (INIT_NUM_PIECES);
}

shared_ptr<Player> ai::getPlayer(const string & color, JsonToStlConverter converter) {
    if (color == "red") {
        auto redGenerator = getGeneratorFor("red", converter);
        return make_shared<RedPlayer>('r', redGenerator);
    }
    auto blackGenerator = getGeneratorFor("black", converter);

    return make_shared<BlackPlayer>('b', blackGenerator, PlayerType::Human);
}


