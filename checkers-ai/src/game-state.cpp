
#include "game-state.h"
using ai::GameState;

GameState::GameState(
        const BoardState & board,
        const Pieces & red,
        const Pieces & black,
        char activePlayerColor):
    boardState(board),
    redPieces(red),
    blackPieces(black),
    activePlayerColor(activePlayerColor) {
    }
