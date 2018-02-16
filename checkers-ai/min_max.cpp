#include "headers/min_max.h"
#include <climits>
#include <algorithm>
using std::max;
using std::min;
#include <iostream>
using std::cout;
using std::endl;


int ai::minimax(std::pair<int, int> move, int depth, char maximizingPlayer, CheckersGame game) {
    cout << "checking move: " << move.first << ", " << move.second << endl;
    game.board.make(move);
    game.reactTo(move);
    game.swapPlayers();

    if (depth == 0) {
        return game.getNumPiecesFor(maximizingPlayer);
    }


    if (game.activePlayer->getColor() == maximizingPlayer) {
        int best = INT_MIN;

        for (auto currMoves : game.getValidMoves()) {
            auto moveValue = minimax(currMoves, depth - 1, maximizingPlayer, game);
            best = max(moveValue, best);
        }

        return best;
    } else {
        int best = INT_MAX;

        for (auto currMoves : game.getValidMoves()) {
            auto moveValue = minimax(currMoves, depth - 1, maximizingPlayer, game);
            best = min(moveValue, best);
        }

        return best;
    }
}
