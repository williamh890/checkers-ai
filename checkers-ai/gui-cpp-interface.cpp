#include "headers/gui-cpp-interface.h"
using ai::GuiCppInterface;

#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/board.h"
using ai::Board;

#include <unistd.h>

#include <vector>
using std::vector;

GuiCppInterface::GuiCppInterface(const CheckersGame & game): game(game) {
}

void GuiCppInterface::play(){
    game.makeRandomValidAction();
    game.swapPlayers();
    usleep(10000);
}

bool GuiCppInterface::isInvalid(const CheckersGame::MovePackage & move) {
    return game.isInvalid(move);
}

bool GuiCppInterface::isInvalid(const CheckersGame::JumpPackage & jump) {
    return game.isInvalid(jump);
}

Board::BoardType GuiCppInterface::getBoard() {
    return game.getBoard();
}

const char GuiCppInterface::getActivePlayerColor() {
    return game.getActivePlayerColor();
}

const char GuiCppInterface::getInactivePlayerColor() {
  return game.getInactivePlayerColor();
}

void GuiCppInterface::makeJump(const CheckersGame::JumpPackage & jump) {
    return game.makeJump(jump);
}

bool GuiCppInterface::areJumps() {
    return game.areJumps();
}

bool GuiCppInterface::areMoves(){
  return game.areMoves();
}

void GuiCppInterface::makeMove(const CheckersGame::MovePackage & move) {
    return game.makeMove(move);
}

void GuiCppInterface::replayJump(const CheckersGame::JumpPackage & jump){
  game.replayJump(jump);
}

void GuiCppInterface::replayMove(const CheckersGame::MovePackage & move){
  game.replayMove(move);
}
void GuiCppInterface::swapPlayers() {
  game.swapPlayers();
}

vector<vector<int>> GuiCppInterface::getGame(){
  return game.getGame();
}
