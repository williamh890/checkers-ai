#include "headers/gui-cpp-interface.h"
using ai::GuiCppInterface;

#include "headers/checkers-game.h"
using ai::CheckersGame;

#include "headers/board.h"
using ai::Board;

GuiCppInterface::GuiCppInterface(const CheckersGame & game): game(game) {
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

void GuiCppInterface::makeJump(const CheckersGame::JumpPackage & jump) {
    return game.makeJump(jump);
}

void GuiCppInterface::makeMove(const CheckersGame::MovePackage & move) {
    return game.makeMove(move);
}

void GuiCppInterface::swapPlayers() {
    return game.swapPlayers();
}

bool GuiCppInterface::areJumps() {
    return game.areJumps();
}
