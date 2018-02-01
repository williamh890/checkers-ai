#include "../headers/utils.h"
using ai::spaceToPosition;
using ai::positionToSpace;

#include "../headers/models.h"
using ai::Position;

#include "../headers/consts.h"
using ai::TOTAL_NUM_PIECES;

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
        auto c1 = spaceToPosition(0);
        auto c2 = spaceToPosition(14);
        auto c3 = spaceToPosition(31);

        REQUIRE((c1.row == 0 and c1.col == 1));
        REQUIRE((c2.row == 3 and c2.col == 4));
        REQUIRE((c3.row == 7 and c3.col == 6));
    }

    SECTION("Test positionToSpace") {
        auto s1 = positionToSpace(Position(0, 1));
        auto s2 = positionToSpace(Position(5, 0));
        auto s3 = positionToSpace(Position(6, 7));

        REQUIRE((s1 == 0));
        REQUIRE((s2 == 20));
        REQUIRE((s3 == 27));
    }

    SECTION("Test convertions") {
        for (auto space = 0; space < TOTAL_NUM_PIECES; ++space) {
            auto pos = spaceToPosition(space);
            auto converted = positionToSpace(pos);

            REQUIRE(space == converted);
        }

    }
}
