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

            bool isInvalid(const CheckersGame::MovePackage & move);
            bool isInvalid(const CheckersGame::JumpPackage & jump);

            std::vector<char> getBoard(); // may not need
            const char getActivePlayerColor();

            void makeJump(const CheckersGame::JumpPackage & jump);
            void makeMove(const CheckersGame::MovePackage & move);

            void swapPlayers(); // may not need
            bool areJumps();
    };
}

#endif
