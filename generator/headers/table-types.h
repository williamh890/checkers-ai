#ifndef TABLE_TYPES_H
#define TABLE_TYPES_D

#include "jump.h"
using jump::Jump;

#include <vector>
using std::vector;

namespace table_types {
    using MoveTableType = vector<vector<int>>;
    using JumpTableType = vector<vector<Jump>>;
}

#endif
