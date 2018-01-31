#ifndef BOARD_H
#define BOARD_H

#include "player.h"
// ai::Player

#include <vector>
// std::vector
#include <memory>
// std::shared_prt

namespace ai {
    class Board {
        private:
            std::vector<char> boardState;

        public:
            Board();
            void addPiecesFor(const std::shared_ptr<Player> & player);
            void print();

        private:
            std::vector<std::vector<char>> getEmptyBoard();
    };

    Board getBoard();
}

#endif
