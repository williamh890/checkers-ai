#include "../headers/json-to-stl.h"
using ai::loadMoveTableFrom;

#include "catch.hpp"

TEST_CASE("testing loading move table from json."){
    auto json = loadMoveTableFrom("test.json");
}
