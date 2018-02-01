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

Board::Board(): boardState(vector<char>(32, ' ')) {};

void Board::addPiecesFor(const shared_ptr<Player> & player) {
    for (const auto & piece: player->getPieces()) {
        boardState[piece.space] = piece.color;
    }
}

void Board::print() {
    auto spaces = getEmptyBoard();

    for (auto i = 0; i < boardState.size(); ++i) {
        auto pos = spaceToGridSquare(i);

        spaces[pos.row][pos.col] = boardState[i];
    }

    const string boardNums = "     0   1   2   3   4   5   6   7  ";
    const string spacerRow = "   +---+---+---+---+---+---+---+---+";

    cout << boardNums << endl;
    auto rowCounter = 0;
    for ( auto row : spaces ) {
        cout << spacerRow << endl;

        cout << " " << rowCounter << " ";
        for (auto space : row) {
            cout << "| " << space << " ";
        }
        cout << "|" << endl;

        ++rowCounter;
    }
    cout << spacerRow << endl;
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
