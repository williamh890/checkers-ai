#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "table-types.h"
// ai::MoveTableType
// ai::JumpTableType
#include "json-to-stl.h"
// ai::JsonToStlConverter

#include <string>
// std::string
#include <vector>
// std::vector

namespace ai {
    class MoveGenerator {
        private:
            MoveTableType moves;
            JumpTableType jumps;

        public:
            MoveGenerator();
            MoveGenerator(const MoveTableType & moves, const JumpTableType & jumps);
            // ~MoveGenerator();

            std::vector<int> getMoves(int space) const;
            std::vector<Jump> getJumps(int space) const;
    };

    MoveGenerator getGeneratorFor(const std::string & color, const JsonToStlConverter & converter);
}

#endif
