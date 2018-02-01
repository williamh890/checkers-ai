#ifndef BOARD_H
#define BOARD_H

#include "player.h"
// ai::Player
#include "table-types.h"
// ai::MoveTableType
#include "models.h"
// ai::pos

#include <vector>
// std::vector
#include <memory>
// std::shared_prt
#include <utility>
// std::pair

namespace ai {
    class Board {
        private:
            std::vector<char> boardState;

        public:
            Board();
            void addPiecesFor(const std::shared_ptr<Player> & player);

            const std::vector<std::pair<int, int>> getValidMovesFor(const std::shared_ptr<Player> & player) const;
        private:
            bool hasPieceAt(int space) const;

        public:
            std::string toString();
        private:
            std::vector<std::vector<char>> getEmptyBoard();
    };

    Board getBoard();
}

#endif
