#include "headers/checkers-game.h"
using ai::CheckersGame;
using ai::loadMoveTableFrom;

#include "headers/json-to-stl.h"
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;

#include "headers/models.h"
using ai::Position;
using ai::Piece;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getGeneratorFor("black", converter);
    auto black = getGeneratorFor("red", converter);

    return CheckersGame(red, black);
}

CheckersGame::CheckersGame(
        MoveGenerator red,
        MoveGenerator black): redGenerator(red), blackGenerator(black) {
    this->initPieces();
}

void CheckersGame::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (this->isInFirst3Rows(space)) {
            Piece newPiece = Piece('r', space);
            this->redPieces.push_back(newPiece);
        }

        if (this->isInLast3Rows(space)) {
            Piece newPiece = Piece('b', space);
            this->blackPieces.push_back(newPiece);
        }
    }
}

bool CheckersGame::isInFirst3Rows(int space) {
    return space < (this->INIT_NUM_PIECES);
}

bool CheckersGame::isInLast3Rows(int space) {
    return space >= (this->TOTAL_NUM_PIECES - this->INIT_NUM_PIECES);
}

string CheckersGame::printBoard() {
    string board = "";

    auto spaces = this->getEmptyBoard();

    for (auto piece: this->blackPieces) {
        auto pos = this->spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    for (auto piece: redPieces) {
        auto pos = this->spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    string spacerRow = "+---+---+---+---+---+---+---+---+";
    for ( auto row : spaces ) {
        cout << spacerRow << endl;

        for (auto space : row) {
            cout << "| " << space << " ";
        }
        cout << "|" << endl;

    }
    cout << spacerRow << endl;

    return board;
}

vector<vector<char>> CheckersGame::getEmptyBoard() {
    vector<vector<char>> board;

    for (auto r = 0; r < this->ROWS; ++r) {
        auto row = vector<char>(this->ROWS, ' ');
        board.push_back(row);
    }

    return board;
}

Position CheckersGame::spaceToGridSquare(int space) {
    auto row = space / 4;
    auto col = space % 4;

    auto offset = (row % 2) ? 0 : 1;

    return Position(row, (2 * col) + offset);
}

