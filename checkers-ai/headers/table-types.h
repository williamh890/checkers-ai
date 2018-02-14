#ifndef TABLE_TYPES_H
#define TABLE_TYPES_D

#include "models.h"
// ai::Jump
#include <vector>
// std::vector

namespace ai {
    using MoveTableType = std::vector<std::vector<int>>;
    using JumpTableType = std::vector<std::vector<ai::Jump>>;
}

#endif
