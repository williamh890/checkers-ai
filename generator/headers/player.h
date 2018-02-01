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
    enum class PlayerType {
        Human, Computer
    };

    class Player {
        private:
            char color;
            std::vector<Piece> pieces;
            MoveGenerator generator;
            PlayerType playerType;

        public:
            Player() = default;
            Player(char color, MoveGenerator generator, PlayerType type);

            const std::vector<Piece> getPieces() const;
            const char getColor() const;
            const PlayerType getPlayerType() const;

            void updatePieces(const std::pair<int, int> & move);
            void updatePieces(const std::pair<int, Jump> & jump);
            void removePieceAt(int space);

            std::vector<Jump> getJumpsFor(const Piece & piece) const;
            std::vector<int> getMovesFor(const Piece & piece) const;

            std::string jumpsToString() const;
            std::string movesToString() const;

        protected:
            void initPieces();
        private:
            virtual bool isInitialSpace(int space) const = 0;
    };

    class RedPlayer: public Player {
        public:
            RedPlayer(char color, MoveGenerator generator, PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
    };

    class BlackPlayer: public Player {
        public:
            BlackPlayer(char color, MoveGenerator generator, PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
    };

    std::shared_ptr<Player> getPlayer(const std::string & color, JsonToStlConverter converter);
}

#endif
