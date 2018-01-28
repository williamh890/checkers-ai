#ifndef JSON_TO_STL_H
#define JSON_TO_STL_H

#include "table-types.h"
#include "jump.h"

#include "json.hpp"
#include <string>
#include <vector>

nlohmann::json loadMoveTableFrom(const std::string & moveTableFilename);

class JsonToStlConverter {
    private:
        nlohmann::json moveTable;

    public:
        JsonToStlConverter(nlohmann::json table);

        MoveTableType getMovesFor(const std::string & color);
        JumpTableType getJumpsFor(const std::string & color);

    private:
        MoveTableType movesToStlContainer(const nlohmann::json & movesJson);
        JumpTableType jumpsToStlContainer(const nlohmann::json & jumpsJson);

        std::vector<Jump> getJumpsFromJson(const nlohmann::json & jumpsForSpace);
        std::vector<int> getMovesFromJson(const nlohmann::json & movesForSpace);

        int strToInt(const std::string & s);
};

#endif
