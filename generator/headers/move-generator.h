#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "table-types.h"
// ai::MoveTableType
// ai::JumpTableType
#include "json-to-stl.h"
// ai::JsonToStlConverter

#include <string>

namespace ai {
    class MoveGenerator {
        private:
            MoveTableType moves;
            JumpTableType jumps;

        public:
            MoveGenerator(const MoveTableType & moves, const JumpTableType & jumps);

            auto getMoves(int space);
            auto getJumps(int space);
    };

    MoveGenerator getGeneratorFor(const std::string & color, const JsonToStlConverter & converter);
}

#endif
