#ifndef GUI_CPP_INTERFACE
#define GUI_CPP_INTERFACE

#include "checkers-game.h"
// ai::CheckersGame

namespace ai {
    class GuiCppInterface {
        private:
            CheckersGame game;
        public:
            GuiCppInterface() = default;
            GuiCppInterface(const CheckersGame & game);

            void play();

            bool isInvalid(const CheckersGame::MovePackage & move);
            bool isInvalid(const CheckersGame::JumpPackage & jump);

            std::vector<char> getBoard(); // may not need
            const char getActivePlayerColor();
            const char getInactivePlayerColor();

            void makeJump(const CheckersGame::JumpPackage & jump);
            void makeMove(const CheckersGame::MovePackage & move);

            void replayJump(const CheckersGame::JumpPackage & jump);
            void replayMove(const CheckersGame::MovePackage & move);

            void swapPlayers(); // may not need
            bool areJumps();
            bool areMoves();

            std::vector<std::vector<int>> getGame();
    };
}

#endif
