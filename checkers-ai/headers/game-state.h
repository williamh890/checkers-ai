#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "models.h"

#include <vector>
// std::vector

namespace ai {
    using Pieces = std::vector<Piece>;
    using BoardState = std::vector<char>;

    struct GameState {
        BoardState boardState;
        Pieces redPieces;
        Pieces blackPieces;
        char activePlayerColor;

        GameState() = default;
        GameState(
            const BoardState & board,
            const Pieces & red,
            const Pieces & black,
            char activePlayerColor);
    };
} // namespace ai

#endif // GAME_STATE_H
