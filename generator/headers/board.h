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
    enum class Action {
        Move, Jump
    };

    class Board {
        private:
            std::vector<char> boardState;

        public:
            Board();
            void addPiecesFor(const std::shared_ptr<Player> & player);

            const std::vector<std::pair<int, int>> getValidMovesFor(const std::shared_ptr<Player> & player) const;
            const std::vector<std::pair<int, Jump>> getValidJumpsFor(const std::shared_ptr<Player> & player) const;

            const Action make(const std::pair<int, Jump> jump);
            const Action make(const std::pair<int, int> move);

        private:
            bool hasPieceAt(int space) const;
            bool hasOpposingPieceAt(const Jump & jump, char color) const;
            bool destinationIsNotEmpty(const Jump & jump) const;
            void UpdatePiece(const int & index, const char & newVal);

        public:
            std::string toString();
            char operator[](const int & index) const;
            std::vector<char> getBoardState();
            
        private:
            std::vector<std::vector<char>> getEmptyBoard();
    };

    Board getBoard();
}

#endif
