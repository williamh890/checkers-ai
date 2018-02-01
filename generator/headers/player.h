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

            std::vector<Jump> getJumpsFor(const Piece & piece) const;
            std::vector<int> getMovesFor(const Piece & piece) const;

            std::string jumpsToString() const;
            std::string movesToString() const;

        private:

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
