#ifndef TABLE_TYPES_H
#define TABLE_TYPES_D

#include "jump.h"
using ai::Jump;

#include <vector>
using std::vector;

namespace ai {
    using MoveTableType = vector<vector<int>>;
    using JumpTableType = vector<vector<Jump>>;
}

#endif
