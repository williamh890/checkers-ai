#include "headers/board.h"
using ai::Board;

#include "headers/player.h"
using ai::Player;

#include "headers/utils.h"
using ai::spaceToGridSquare;
using ai::positionToSpace;

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <sstream>
using std::stringstream;


Board::Board(): boardState(vector<char>(32, ' ')) {};

void Board::addPiecesFor(const shared_ptr<Player> & player) {
    for (const auto & piece: player->getPieces()) {
        boardState[piece.space] = piece.color;
    }
}

string Board::toString() {
    auto spaces = getEmptyBoard();

    for (auto i = 0; i < (int)boardState.size(); ++i) {
        auto pos = spaceToGridSquare(i);

        spaces[pos.row][pos.col] = boardState[i];
    }

    const string boardNums = "     0   1   2   3   4   5   6   7  ";
    const string spacerRow = "   +---+---+---+---+---+---+---+---+";

    stringstream ss;
    ss << boardNums << endl;
    auto rowCounter = 0;
    for ( auto row : spaces ) {
        ss << spacerRow << endl;

        ss << " " << rowCounter << " ";
        for (auto space : row) {
            ss << "| " << space << " ";
        }
        ss << "|" << endl;

        ++rowCounter;
    }
    ss << spacerRow << endl;

    return ss.str();
}

Board ai::getBoard() {
    return Board();
}

char Board::at(const Position & pos) const {
    auto i = positionToSpace(pos);
    return boardState[i];
}

vector<vector<char>> Board::getEmptyBoard() {
    vector<vector<char>> board;

    for (auto r = 0; r < 8; ++r) {
        auto row = vector<char>(8, ' ');
        board.push_back(row);
    }

    return board;
}
