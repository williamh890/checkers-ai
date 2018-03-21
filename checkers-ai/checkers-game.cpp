#include "headers/checkers-game.h"
using ai::CheckersGame;
using PlayerPtr = CheckersGame::PlayerPtr;
using MovePackage = CheckersGame::MovePackage;
using JumpPackage = CheckersGame::JumpPackage;
using SeederPtr = CheckersGame::SeederPtr;

#include "headers/seeder.h"
using ai::RandomDeviceSeeder;
using ai::Seeder;
using ai::SRandSeeder;
using ai::getSeeder;

#include "headers/search.h"
using ai::getBestJump;
using ai::getBestMove;

#include "headers/board.h"
using ai::getBoard;
using ai::Board;

#include "headers/player.h"
using ai::Player;
using ai::RedPlayer;
using ai::BlackPlayer;
using ai::getPlayer;
using ai::getNetworkedPlayer;

#include "headers/json-to-stl.h"

#include "headers/move-generator.h"
using ai::getGeneratorFor;
using ai::MoveGenerator;

#include "headers/utils.h"

#include "headers/models.h"
using ai::Jump;
using ai::Position;
using ai::Piece;

#include "headers/game-state.h"
using ai::GameState;

#include "headers/consts.h"
using ai::TOTAL_NUM_SPACES;
using ai::INIT_NUM_PIECES;
using ai::MOVE_LIMIT;

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
#include <climits>

int CheckersGame::SEARCH_DEPTH = 6;

CheckersGame ai::getCheckersGame() {
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getPlayer("red", converter);
    auto black = getPlayer("black", converter);
    auto board = getBoard();

    auto seeder = getSeeder();

    return CheckersGame(board, red, black, seeder);
}

CheckersGame ai::getNetworkedCheckersGame(unsigned int red_id, unsigned int black_id){
    auto table = loadMoveTableFrom("move-table.json");
    auto converter = JsonToStlConverter{table};

    auto red = getNetworkedPlayer("red", converter, red_id);
    auto black = getNetworkedPlayer("black", converter, black_id);
    auto board = getBoard();

    auto seeder = getSeeder();

    return CheckersGame(board, red, black, seeder);
}

CheckersGame::CheckersGame() { };

CheckersGame::CheckersGame(
        const Board & board,
        PlayerPtr red,
        PlayerPtr black,
        SeederPtr & seeder):
    board(board),
    red(red),
    black(black),
    activePlayer(black),
    inactivePlayer(red)
{

    generator = mt19937(seeder->get());

    this->board.addPiecesFor(red);
    this->board.addPiecesFor(black);
}

const char CheckersGame::play() {
    while (++moveCounter < MOVE_LIMIT && (areMoves() || areJumps())) {

        MovePackage move = make_pair(-1, -1);
        JumpPackage jump = make_pair(-1, Jump(-1, -1));

        try {
            if (areJumps()) {
                jump = getJumpFromActivePlayer();
            } else {
                move = getMoveFromActivePlayer();
            }
        }
        catch(length_error & e) {
            break;
        }
        catch(runtime_error & e) {
            continue;
        }

        if (move.first == -1) {
            board.make(jump);
            reactTo(jump);

            auto validJumps = getValidJumpsAt(jump.second.to);

            while (validJumps.size()) {
                //cout << board.toString() << endl;
                auto jump = make_pair(-1, Jump(-1, -1));

                try {
                    jump = getBestJump();
                }
                catch(runtime_error & e) {
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
    cout << toString() << endl;
    cout << "moves in game " << moveCounter << endl;

    return getInactivePlayerColor();
}

void CheckersGame::swapPlayers(){
    swap(activePlayer, inactivePlayer);
}

bool CheckersGame::areJumps(){
    return getValidJumps().size() > 0;
}

bool CheckersGame::areMoves(){
    return getValidMoves().size() > 0;
}

JumpPackage CheckersGame::getJumpFromActivePlayer() {
    return this->getBestJump();
}

MovePackage CheckersGame::getBestMove() {
    return ai::getBestMove(*this, SEARCH_DEPTH);
}

JumpPackage CheckersGame::getBestJump(int space) {
    return ai::getBestJump(*this, SEARCH_DEPTH, space);
}

JumpPackage CheckersGame::getRandomValidJump() {
    auto jumps = getValidJumps();

    auto numJumps = jumps.size();

    uniform_int_distribution<int> distribution(0, numJumps - 1);
    auto rngVal = distribution(generator);
    return jumps[rngVal];
}

JumpPackage CheckersGame::getJumpFromUser() {
    auto inputJump = parseUserInput();

    return getJumpFrom(inputJump);
}

JumpPackage CheckersGame::getJumpFrom(const MovePackage & inputJump) {
    auto validJumps = getValidJumps();

    for (const auto & checkJump: validJumps) {
        if (checkJump.first == inputJump.first and
                checkJump.second.to == inputJump.second) {

            return checkJump;
        }
    }

    throw runtime_error("invalid jump input.");
}

MovePackage CheckersGame::getMoveFromActivePlayer() {
    return getBestMove();
}

MovePackage CheckersGame::getRandomValidMove() {
    auto moves = getValidMoves();
    auto numMoves = moves.size();

    if ( numMoves < 1 ) {
        throw length_error("out of moves");
    }

    uniform_int_distribution<int> distribution(0, numMoves - 1);
    auto rngVal = distribution(generator);

    return moves[rngVal];
}

MovePackage CheckersGame::parseUserInput() {
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

const char CheckersGame::getActivePlayerColor(){
    return activePlayer->getColor();
}

const char CheckersGame::getInactivePlayerColor(){
    return inactivePlayer->getColor();
}

bool CheckersGame::reactTo(const JumpPackage & jump) {
    auto wasPieceCrowned =
        activePlayer->updatePieces(jump, board);

    inactivePlayer->removePieceAt(jump.second.through);

    return wasPieceCrowned;
}

bool CheckersGame::reactTo(const MovePackage & move) {
    auto wasPieceCrowned =
        activePlayer->updatePieces(move, board);

    return wasPieceCrowned;
}

string CheckersGame::toString() {
    return board.toString();
}

GameState CheckersGame::getState() {
    return GameState(
            board.getBoardState(),
            red->getPieces(),
            black->getPieces(),
            activePlayer->getColor()
            );
}

void CheckersGame::setState(GameState & state) {
    board.setBoardState(state.boardState);
    red->setPieces(state.redPieces);
    black->setPieces(state.blackPieces);
    activePlayer = state.activePlayerColor == 'r' ? red : black;
    inactivePlayer = state.activePlayerColor == 'r' ? black : red;
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


int CheckersGame::getNumPiecesFor(char color) {
    return (color == 'r') ? red->getPieces().size() : black->getPieces().size();
}
