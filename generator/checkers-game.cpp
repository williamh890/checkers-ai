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
    initPieces();
}

void CheckersGame::initPieces() {
    for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
        if (isInFirst3Rows(space)) {
            Piece newPiece = Piece('r', space);
            redPieces.push_back(newPiece);
        }

        if (isInLast3Rows(space)) {
            Piece newPiece = Piece('b', space);
            blackPieces.push_back(newPiece);
        }
    }
}

bool CheckersGame::isInFirst3Rows(int space) {
    return space < (INIT_NUM_PIECES);
}

bool CheckersGame::isInLast3Rows(int space) {
    return space >= (TOTAL_NUM_PIECES - INIT_NUM_PIECES);
}

string CheckersGame::printBoard() {
    string board = "";

    auto spaces = getEmptyBoard();

    for (auto piece: blackPieces) {
        auto pos = spaceToGridSquare(piece.space);
        spaces[pos.row][pos.col] = piece.color;
    }

    for (auto piece: redPieces) {
        auto pos = spaceToGridSquare(piece.space);
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

void CheckersGame::printMoves()
{
    cout << "Black Moves: ";
    for ( Piece checker : blackPieces)
    {
        // for (auto move : blackGenerator.getMoves(checker.space))
        // {
        //     cout << "[" << move[0] <<","<<move[1]<<"]";
        // }
        cout << ", ";
    }
    cout << endl;
    cout << "Black Jumps: ";
    for ( Piece checker : blackPieces)
    {
        //cout << blackGenerator.getJumps(checker.space);
        cout << ", ";
    }
    cout << endl;
    cout << "Red Moves: ";
    for ( Piece checker : redPieces)
    {
        //cout << blackGenerator.getMoves(checker.space);
        cout << ", ";
    }
    cout << endl;
    cout << "Red Jumps: ";
    for ( Piece checker : redPieces)
    {
        //cout << blackGenerator.getJumps(checker.space);
        cout << ", ";
    }
    cout << endl;

}

vector<vector<char>> CheckersGame::getEmptyBoard() {
    vector<vector<char>> board;

    for (auto r = 0; r < ROWS; ++r) {
        auto row = vector<char>(ROWS, ' ');
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

