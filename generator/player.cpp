#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;

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



Player::Player(char color, MoveGenerator generator): color(color), generator(generator) {
}

void Player::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (isInitialSpace(space)) {
            Piece newPiece = Piece(color, space);
            pieces.push_back(newPiece);
        }
    }
}

vector<Piece> Player::getPieces() const {
    return pieces;
}

char Player::getColor() const {
    return color;
}

vector<int> Player::getMovesFor(Piece piece) const {
    return generator.getMoves(piece.space);
};

vector<Jump> Player::getJumpsFor(Piece piece) const {
    return generator.getJumps(piece.space);
};

BlackPlayer::BlackPlayer(char color, MoveGenerator generator): Player(color, generator) {
    initPieces();
}

bool BlackPlayer::isInitialSpace(int space) const {
    return space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES);
}

RedPlayer::RedPlayer(char color, MoveGenerator generator): Player(color, generator) {
    initPieces();
}

bool RedPlayer::isInitialSpace(int space) const {
    return space < (INIT_NUM_PIECES);
}

shared_ptr<Player> ai::getPlayer(const string & color, JsonToStlConverter converter) {
    if (color == "black") {
        auto redGenerator = getGeneratorFor("red", converter);
        return make_shared<RedPlayer>('r', redGenerator);
    }

    auto blackGenerator = getGeneratorFor("black", converter);
    return make_shared<BlackPlayer>('b', blackGenerator);
}
