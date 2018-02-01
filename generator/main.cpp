#include "headers/checkers-game.h"
using ai::getCheckersGame;

#include <iostream>
using std::cout;
using std::endl;

/*         0   1   2   3   4   5   6   7
         +---+---+---+---+---+---+---+---+
     0   | X | 0 |   | 1 |   | 2 |   | 3 |
         +---+---+---+---+---+---+---+---+
     1   | 4 |   | 5 |   | 6 |   | 7 |   |
         +---+---+---+---+---+---+---+---+
     2   |   | 8 |   | 9 |   | 10|   | 11|
         +---+---+---+---+---+---+---+---+
     3   | 12|   | 13|   | 14|   | 15|   |
         +---+---+---+---+---+---+---+---+
     4   |   | 16|   | 17|   | 18|   | 19|
         +---+---+---+---+---+---+---+---+
     5   | 20|   | 21|   | 22|   | 23|   |
         +---+---+---+---+---+---+---+---+
     6   |   | 24|   | 25|   | 26|   | 27|
         +---+---+---+---+---+---+---+---+
     7   | 28|   | 29|   | 30|   | 31| X |
         +---+---+---+---+---+---+---+---+
20: to 13, through 16
21: to 12, through 16 to 14, through 17
22: to 13, through 17 to 15, through 18
23: to 14, through 18
*/

int main() {
    auto game = getCheckersGame();
    cout << game.toString() << endl;

    return 0;
}
