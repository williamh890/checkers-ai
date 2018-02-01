#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/board.h"
using ai::Board;

#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;

#include "headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "headers/move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "headers/utils.h"
using ai::spaceToGridSquare;

#include "headers/models.h"
using ai::Jump;
using ai::Position;
using ai::Piece;

#include "headers/consts.h"
using ai::TOTAL_NUM_PIECES;
using ai::INIT_NUM_PIECES;

#include "headers/table-types.h"
using ai::MoveTableType;

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <utility>
using std::move;


CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    return CheckersGame(board, red, black);
}
CheckersGame::CheckersGame(){ };

CheckersGame::CheckersGame(
        const Board & board,
        shared_ptr<Player> red,
        shared_ptr<Player> black
        ): board(board), red(red), black(black) {
    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

std::string CheckersGame::toString() {
    return board.toString();
}

void CheckersGame::printMoves()
{
    printJumpsForColor("black");
}

void CheckersGame::printMovesForColor(const string & color) {
    cout << color << " Moves: " << endl;
    auto pieces = ((color == "black") ? black: red)->getPieces();

    for (const auto & checker : pieces)
    {
        auto s = spaceToGridSquare(checker.space);
        cout << "(" << s.row << ", " << s.col << "): ";
        for (const auto & move: black->getMovesFor(checker)) {
            cout << move << " ";
        }
        cout << "]"<<endl;
    }

}

void CheckersGame::printJumpsForColor(const string & color) {
    cout << color << " Jumps: " << endl;
    auto pieces = ((color == "black") ? black: red)->getPieces();

    for (const auto & checker: pieces) {
        cout << checker.space << ": ";

        for (const auto & jump: black->getJumpsFor(checker)) {
            cout << jump.to << ", " << jump.through << " ";
        }
        cout << endl;
    }
}


MoveTableType CheckersGame::getBlackMoves()
{
    MoveTableType blackMoves;
    bool valid = true;

    for(const auto & piece : black->getPieces())
    {
        auto moves = black->getMovesFor(piece);
        vector<int> black_move;
        for(auto & move : moves)
        {

            for (const auto & other_black: black->getPieces())
            {
                if(move == other_black.space)
                {
                    valid = false;
                    break;
                }
            }
            if(valid)
            {
                for (const auto & other_red: red->getPieces())
                {
                    if(move == other_red.space)
                    {
                        valid = false;
                    }
                }
                if(valid)
                {
                    black_move.push_back(piece.space);
                    black_move.push_back(move);
                }
            }
            valid = true;
        }
        if (black_move.size())
        {
            blackMoves.push_back(black_move);
        }
    }

    return blackMoves;
}
