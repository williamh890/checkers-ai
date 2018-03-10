#ifndef PLAYER_H
#define PLAYER_H

#include "consts.h"
// ai::PlayerType
#include "models.h"
// ai::Piece
// ai::Jump

#include "board.h"
// ai::Board;

#include "move-generator.h"
// ai::MoveGenerator

#include "network.h"
// ai::Network;

#include <vector>
// std::vector

#include<functional>
//std::function


namespace ai {
    class Board;//forward declaration
    class Network; //forward declaration
    class MiniMaxHelper;//forward declaration
    class Player {
        private:
            PlayerType playerType;
            char color;
            Network network;
            std::vector<Piece> pieces;
            MoveGenerator generator;
            MoveGenerator kingGenerator;
            int base_case_color_factor;

        public:
            Player() = default;

            Player(char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    PlayerType type
                  );

            Player(char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    Network & network,
                    PlayerType type
                  );

            std::function<float(MiniMaxHelper&)> baseCase;

            const std::vector<Piece> getPieces() const;
            void setPieces(const std::vector<Piece> & pieces);
            const char getColor() const;
            const PlayerType getPlayerType() const;

            bool updatePieces(const std::pair<int, int> & move, ai::Board & board);
            bool updatePieces(const std::pair<int, Jump> & jump, ai::Board & board);
            void removePieceAt(int space);

            std::vector<Jump> getJumpsFor(const Piece & piece) const;
            std::vector<int> getMovesFor(const Piece & piece) const;

            std::string jumpsToString() const;
            std::string movesToString() const;

        protected:
            void initPieces();
        private:
            virtual bool isInitialSpace(int space) const = 0;
            virtual bool shouldBeCrowned(const Piece & piece) const = 0;
            void Crown(Piece & piece);
    };

    class RedPlayer: public Player {
        public:
            RedPlayer(
                    char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    PlayerType type);
            RedPlayer(
                    char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    Network & network,
                    PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
            bool shouldBeCrowned(const Piece & piece) const override;
    };

    class BlackPlayer: public Player {
        public:
            BlackPlayer(
                    char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    PlayerType type);
            BlackPlayer(
                    char color,
                    const MoveGenerator & generator,
                    const MoveGenerator & kingGenerator,
                    Network & network,
                    PlayerType type);
        private:
            bool isInitialSpace(int space) const override;
            bool shouldBeCrowned(const Piece & piece) const override;
    };

    std::shared_ptr<Player> getPlayer(const std::string & color,
                                      JsonToStlConverter converter);
    std::shared_ptr<Player> getNetworkedPlayer(
                                               const std::string & color,
                                               JsonToStlConverter converter,
                                               uint network_id);
}

#endif
