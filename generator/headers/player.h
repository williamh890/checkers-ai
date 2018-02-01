#ifndef PLAYER_H
#define PLAYER_H

#include "models.h"
// ai::Piece
// ai::Jump
#include "move-generator.h"
// ai::MoveGenerator

#include <vector>
// std::vector

namespace ai {
    class Player {
        private:
            char color;
            std::vector<Piece> pieces;
            MoveGenerator generator;

        public:
            Player();
            Player(char color, MoveGenerator generator);
            const std::vector<Piece> getPieces() const;
            const char getColor() const;

            const std::vector<int> getMovesFor(Piece piece) const;
            const std::vector<Jump> getJumpsFor(Piece piece) const;

        protected:
            void initPieces();
        private:
            virtual bool isInitialSpace(int space) const = 0;
    };

    class RedPlayer: public Player {
        public:
            RedPlayer(char color, MoveGenerator generator);
        private:
            bool isInitialSpace(int space) const override;
    };

    class BlackPlayer: public Player {
        public:
            BlackPlayer(char color, MoveGenerator generator);
        private:
            bool isInitialSpace(int space) const override;
    };


    std::shared_ptr<Player> getPlayer(const std::string & color, JsonToStlConverter converter);
}

#endif