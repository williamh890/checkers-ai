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
using ai::spaceToPosition;

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
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <memory>
using std::make_shared;
using std::shared_ptr;
#include <utility>
using std::pair;
using std::make_pair;


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
        ): board(board), red(red), black(black), activePlayer(black) {
    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

void CheckersGame::play() {
    while (true) {
        cout << toString() << endl;

        pair<int, int> move;

        try {
            move = getMoveFromActivePlayer();
        } catch(...) {
            cout << "Invlaid Move" << endl;
            continue;
        }

        cout << move.first << ", " << move.second << endl;

        auto action = board.make(move);
        reactTo(action);

        cout << toString() << endl;
        break;
    }
}

pair<int, int> CheckersGame::getMoveFromActivePlayer() {
    if (activePlayer->getPlayerType() == PlayerType::COMPUTER) {
        return getRandomValidMove();
    }

    return getMoveFromUser();
}

pair<int, int> CheckersGame::getRandomValidMove() {
    auto moves = getValidMoves();

    return moves[0];
}

pair<int, int> CheckersGame::getMoveFromUser() {
    auto moves = getValidMoves();
    cout << "Enter move (startRow startCol endRow endCol): " << endl;

    return moves[0];
}

vector<pair<int, int>> CheckersGame::getValidMoves() {
    auto validMoves = board.getValidMovesFor(activePlayer);

    return validMoves;
}

void CheckersGame::reactTo(const Action & action) {
    if (action == Action::Move) {
        cout << "piece was moved" << endl;
    }
}

string CheckersGame::toString() {
    return board.toString();
}

//returns vector<vector<int with format {piece space, valid move, valid move}
MoveTableType CheckersGame::getBlackMoves()
{
    MoveTableType blackMoves;

    for(const auto & piece : black->getPieces())
    {
        auto moves = black->getMovesFor(piece);
        cout << "debug: moves size:" <<moves.size() <<endl;//debug
        vector<int> black_move;
        black_move.push_back(piece.space);
        for(const auto & move : moves)
        {
            cout << "debug: move" << move <<endl;//debug
            if(board[move] == ' ')
            {
                black_move.push_back(move);
            }
        }
        if (black_move.size()>1)
        {
            blackMoves.push_back(black_move);
        }
    }

    return blackMoves;
}

MoveTableType CheckersGame::getRedMoves()
{
    MoveTableType redmoves;
    return redmoves;
}
