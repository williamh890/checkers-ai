#include "../headers/move-generator.h"
using ai::getGeneratorFor;
using ai::getKingGenerator;
using ai::MoveGenerator;
#include "../headers/json-to-stl.h"
using ai::loadMoveTableFrom;
using ai::JsonToStlConverter;

#include "catch.hpp"

TEST_CASE("testing move generator class") {
    auto json = loadMoveTableFrom("tests/test.json");
    auto converter = JsonToStlConverter(json);

}
