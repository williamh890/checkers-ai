#include "headers/checkers-game.h"
using ai::getCheckersGame;

/*
         +---+---+---+---+---+---+---+---+
         | X | 0 |   | 1 |   | 2 |   | 3 |
         +---+---+---+---+---+---+---+---+
         | 4 |   | 5 |   | 6 |   | 7 |   |
         +---+---+---+---+---+---+---+---+
         |   | 8 |   | 9 |   | 10|   | 11|
         +---+---+---+---+---+---+---+---+
         | 12|   | 13|   | 14|   | 15|   |
         +---+---+---+---+---+---+---+---+
         |   | 16|   | 17|   | 18|   | 19|
         +---+---+---+---+---+---+---+---+
         | 20|   | 21|   | 22|   | 23|   |
         +---+---+---+---+---+---+---+---+
         |   | 24|   | 25|   | 26|   | 27|
         +---+---+---+---+---+---+---+---+
         | 28|   | 29|   | 30|   | 31| X |
         +---+---+---+---+---+---+---+---+
*/

int main() {
    auto game = getCheckersGame();
    game.printBoard();
    game.printMoves();

    return 0;
}