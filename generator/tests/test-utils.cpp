#include "../headers/utils.h"
using ai::spaceToGridSquare;
using ai::gridToSpace;

#include "../headers/models.h"
using ai::Position;


#include "catch.hpp"

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
*/

TEST_CASE("Test util functions") {
    SECTION("Test space to grid") {
        auto c1 = spaceToGridSquare(0);
        auto c2 = spaceToGridSquare(14);
        auto c3 = spaceToGridSquare(31);

        REQUIRE((c1.row == 0 and c1.col == 1));
        REQUIRE((c2.row == 3 and c2.col == 4));
        REQUIRE((c3.row == 7 and c3.col == 6));
    }

    SECTION("Test gridToSpace") {

    }

    SECTION("Test convertions") {

    }
}
