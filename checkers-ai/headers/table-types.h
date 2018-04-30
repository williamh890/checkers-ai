#ifndef TABLE_TYPES_H
#define TABLE_TYPES_H

#include "models.h"
// ai::Jump
#include <vector>
// std::vector

namespace ai {
using MoveTableType = std::vector<std::vector<int>>;
using JumpTableType = std::vector<std::vector<ai::Jump>>;
}  // namespace ai

#endif  // TABLE_TYPES_H
