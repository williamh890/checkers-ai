#ifndef GET_CHECKERS_GAME_H
#define GET_CHECKERS_GAME_H

#include "checkers-game.h"

namespace ai {
    CheckersGame getCheckersGame();

    CheckersGame getNetworkedCheckersGame(
            unsigned int red_id,
            unsigned int black_id);

    CheckersGame getNetworkVPieceCountCheckersGame(
            unsigned int network_id,
            char networked_player);
}

#endif
