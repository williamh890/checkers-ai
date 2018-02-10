#include "headers/checkers-game.h"
using ai::CheckersGame;
using PlayerPtr = CheckersGame::PlayerPtr;
using MovePackage = CheckersGame::MovePackage;
using JumpPackage = CheckersGame::JumpPackage;

#include "headers/seeder.h"
using ai::RandomDeviceSeeder;
using ai::Seeder;
using ai::SRandSeeder;

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
using ai::TOTAL_NUM_SPACES;
using ai::INIT_NUM_PIECES;

#include "headers/table-types.h"
using ai::MoveTableType;

#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
using std::make_pair;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <exception>
using std::runtime_error;
using std::length_error;
#include <random>
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;


CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    shared_ptr<Seeder> seeder;
    if (Settings::SEEDING_METHOD == "random_device") {
        seeder = make_shared<RandomDeviceSeeder>();
    }
    if (Settings::SEEDING_METHOD == "time") {
        seeder = make_shared<SRandSeeder>();
    }

    return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getSeedlessCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    return CheckersGame(board, red, black);
  }

CheckersGame::CheckersGame() { };

CheckersGame::CheckersGame(
        const Board & board,
        PlayerPtr red,
        PlayerPtr black):board(board), red(red), black(black){ }

CheckersGame::CheckersGame(
        const Board & board,
        PlayerPtr red,
        PlayerPtr black,
        const shared_ptr<Seeder> & seeder): board(board), red(red), black(black), activePlayer(black), inactivePlayer(red) {

    generator = mt19937(seeder->get());

    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

void CheckersGame::play() {
    while (moveCounter++ < 100) {
        cout << toString() << endl;

        MovePackage move = make_pair(-1, -1);
        JumpPackage jump = make_pair(-1, Jump(-1, -1));

        try {
            if (getValidJumps().size()) {
                jump = getJumpFromActivePlayer();
            } else {
                move = getMoveFromActivePlayer();
            }
        }
        catch(length_error & e) {
            cout << "Out of moves" << endl;
            cout << e.what() << endl;
            break;
        }
        catch(runtime_error & e) {
            cout << "Invalid Move..." << endl;
            cout << e.what() << endl;
            continue;
        }

        cout << activePlayer->getColor() << ": "
            << spaceToPosition(move.first).toString() << ", "
            << spaceToPosition(move.second).toString()
            << endl;

        if (move.first == -1) {
            board.make(jump);
            reactTo(jump);

            auto validJumps = getValidJumpsAt(jump.second.to);

            cout << board.toString() << endl;
            cout << "jump piece has " << validJumps.size() << " possible jumps." << endl;

            while (validJumps.size()) {
                auto jump = make_pair(-1, Jump(-1, -1));

                try {
                    jump = getJumpFromActivePlayer();
                }
                catch(runtime_error & e) {
                    cout << "Invlaid Jump..." << endl;
                    continue;
                }

                board.make(jump);
                reactTo(jump);

                validJumps = getValidJumpsAt(jump.second.to);
            }
        }
        else {
            board.make(move);
            reactTo(move);
        }

        swapPlayers();
    }

    cout << "moves in game " << moveCounter << endl;
}

void CheckersGame::swapPlayers(){
    swap(activePlayer, inactivePlayer);
}

bool CheckersGame::areJumps(){
  return getValidJumps().size();
}
JumpPackage CheckersGame::getJumpFromActivePlayer() {
    if (activePlayer->getPlayerType() == PlayerType::Computer) {
        return getRandomValidJump();
    }

    return getJumpFromUser();
}

JumpPackage CheckersGame::getRandomValidJump() {
    auto jumps = getValidJumps();

    auto numJumps = jumps.size();
    cout << "number of jumps: " << numJumps << endl;

    uniform_int_distribution<int> distribution(0, numJumps - 1);
    auto rngVal = distribution(generator);
    cout << "rngValue: " << rngVal << endl;
    return jumps[rngVal];
}

JumpPackage CheckersGame::getJumpFromUser() {
    auto inputJump = parseUserInput();

    return getJumpFrom(inputJump);
}

JumpPackage CheckersGame::getJumpFrom(const MovePackage & inputJump) {
    auto validJumps = getValidJumps();
    cout << "number of jumps: " << validJumps.size() << endl;

    for (const auto & checkJump: validJumps) {
        if (checkJump.first == inputJump.first and
                checkJump.second.to == inputJump.second) {

            return checkJump;
        }
    }

    throw runtime_error("invalid jump input.");
}

MovePackage CheckersGame::getMoveFromActivePlayer() {
    if (activePlayer->getPlayerType() == PlayerType::Computer) {
        return getRandomValidMove();
    }

    return getMoveFromUser();
}

MovePackage CheckersGame::getRandomValidMove() {
    auto moves = getValidMoves();
    auto numMoves = moves.size();

    cout << "number of moves: " << numMoves << endl;
    if ( numMoves < 1 ) {
        throw length_error("out of moves");
    }

    uniform_int_distribution<int> distribution(0, numMoves - 1);
    auto rngVal = distribution(generator);

    return moves[rngVal];
}

MovePackage CheckersGame::parseUserInput() {
    cout << "Enter move (startRow startCol endRow endCol): ";

    string in;
    getline(cin, in);
    istringstream iss(in);

    int startRow, startCol, endRow, endCol;
    iss >> startRow >> startCol >> endRow >> endCol;

    auto startPos = Position(startRow, startCol);
    auto endPos = Position(endRow, endCol);

    auto start = positionToSpace(startPos);
    auto end = positionToSpace(endPos);

    return make_pair(start, end);
}

MovePackage CheckersGame::getMoveFromUser() {
    auto move = parseUserInput();

    for (auto piece : activePlayer->getPieces()) {
        if (piece.space == move.second) {
            throw runtime_error("cannot move over own piece.");
        }
    }

    return move;
}

bool CheckersGame::isInvalid(const MovePackage & move) {
    auto validMoves = getValidMoves();

    for (const auto & validMove : validMoves) {
        if (validMove == move) {
            return false;
        }
    }

    return true;
}

bool CheckersGame::isInvalid(const JumpPackage & jump) {
    auto validJumps = getValidJumps();

    for (const auto & validJump : validJumps) {
        if (validJump == jump) {
            return false;
        }
    }

    return true;
}

vector<MovePackage> CheckersGame::getValidMoves() {
    auto validMoves = board.getValidMovesFor(activePlayer);

    return validMoves;
}

vector<JumpPackage> CheckersGame::getValidJumps() {
    auto validJumps = board.getValidJumpsFor(activePlayer);

    return validJumps;
}

vector<JumpPackage> CheckersGame::getValidJumpsAt(int space) {
    auto validJumps = board.getValidJumpsFor(activePlayer);
    auto jumpsAtSpace = vector<JumpPackage>{};

    for (auto & jump: validJumps) {
        if (jump.first == space) {
            jumpsAtSpace.push_back(jump);
        }
    }

    return jumpsAtSpace;
}
// Accessor functions for gui/cython wrapper
vector<char> CheckersGame::getBoard(){
    return board.getBoardState();
}

const char CheckersGame::getActivePlayerColor(){
    return activePlayer->getColor();
}

// TODO! Want to combine these reactTo functions in the future
void CheckersGame::reactTo(const JumpPackage & jump) {
    activePlayer->updatePieces(jump, board);
    inactivePlayer->removePieceAt(jump.second.through);
    cout << "piece was jumped" << endl;
}

void CheckersGame::reactTo(const MovePackage & move) {
    activePlayer->updatePieces(move, board);
    cout << "piece was moved" << endl;
}

string CheckersGame::toString() {
    return board.toString();
}


void CheckersGame::makeJump(const JumpPackage & jump){
    try {
        board.make(jump);
        reactTo(jump);
        if (not areJumps()){
            swapPlayers();
            makeRandomValidAction();
        }
    } catch(length_error & e) {
        cout << "Out of moves" << endl;
        cout << e.what() << endl;
    }

}

void CheckersGame::makeMove(const MovePackage & move){
    board.make(move);
    reactTo(move);

    swapPlayers();
}

void CheckersGame::makeRandomValidAction(){
    if (areJumps()){
        auto jump = getRandomValidJump();
        board.make(jump);
        reactTo(jump);

        if (areJumps()){
            makeRandomValidAction();
        }

        return;
    }

    auto move = getRandomValidMove();
    board.make(move);
    reactTo(move);
}
