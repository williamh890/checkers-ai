#ifndef JSON_TO_STL_H
#define JSON_TO_STL_H

#include "table-types.h"
// ai::MoveTableType
// ai::JumpTableType
#include "models.h"
// ai::Jump

#include "json.hpp"
#include <string>
#include <vector>

namespace ai {
    nlohmann::json loadMoveTableFrom(const std::string & moveTableFilename);

    class JsonToStlConverter {
        private:
            nlohmann::json moveTable;

        public:
            JsonToStlConverter(nlohmann::json table);

            MoveTableType getMovesFor(const std::string & color) const;
            JumpTableType getJumpsFor(const std::string & color) const;

        private:
            MoveTableType movesToStlContainer(const nlohmann::json & movesJson) const;
            JumpTableType jumpsToStlContainer(const nlohmann::json & jumpsJson) const;

            std::vector<Jump> getJumpsFromJson(const nlohmann::json & jumpsForSpace) const;
            std::vector<int> getMovesFromJson(const nlohmann::json & movesForSpace) const;

            int strToInt(const std::string & s) const;
    };
}

#endif
