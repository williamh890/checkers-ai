#ifndef PLAYER_H
#define PLAYER_H

#include "consts.h"
// ai::PlayerType
#include "models.h"
// ai::Piece
// ai::Jump

#include "board.h"
//using ai::Board;

#include "move-generator.h"
// ai::MoveGenerator

#include "network.h"
//using ai::Network;
#include <vector>
// std::vector


namespace ai {

    class Board;//forward declaration
    class Network;
    class MiniMaxHelper;
    class Player {
        private:
            PlayerType playerType;
            char color;
            ai::Network network;
            std::vector<Piece> pieces;
            MoveGenerator generator;
            MoveGenerator kingGenerator;

        public:
            Player() = default;
            Player(char color, const MoveGenerator & generator, const MoveGenerator & kingGenerator, PlayerType type);
            Player(char color, const MoveGenerator & generator, const MoveGenerator & kingGenerator, ai::Network & network, PlayerType type);
            const std::vector<Piece> getPieces() const;
            void setPieces(const std::vector<Piece> & pieces);
            const char getColor() const;
            const PlayerType getPlayerType() const;

            void updatePieces(const std::pair<int, int> & move, ai::Board & board);
            void updatePieces(const std::pair<int, Jump> & jump, ai::Board & board);
            void removePieceAt(int space);

            std::vector<Jump> getJumpsFor(const Piece & piece) const;
            std::vector<int> getMovesFor(const Piece & piece) const;

            std::string jumpsToString() const;
            std::string movesToString() const;
            std::function<int(MiniMaxHelper&)> baseCase;

        protected:
            void initPieces();
        private:
            virtual bool isInitialSpace(int space) const = 0;
            virtual bool shouldBeCrowned(const Piece & piece) const = 0;
            void Crown(Piece & piece);
    };

    class RedPlayer: public Player {
        public:
            RedPlayer(char color, const MoveGenerator & generator, const MoveGenerator & kingGenerator, PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
            bool shouldBeCrowned(const Piece & piece) const override ;
    };

    class BlackPlayer: public Player {
        public:
            BlackPlayer(char color, const MoveGenerator & generator, const MoveGenerator & kingGenerator, PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
            bool shouldBeCrowned(const Piece & piece) const override ;
    };

    std::shared_ptr<Player> getPlayer(const std::string & color, JsonToStlConverter converter);
}

#endif
