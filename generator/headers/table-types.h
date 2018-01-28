#ifndef TABLE_TYPES_H
#define TABLE_TYPES_D

#include "jump.h"

#include <unordered_map>
using std::unordered_map;
#include <vector>
using std::vector;

using MoveTableType = unordered_map<int, vector<int>>;
using JumpTableType = unordered_map<int, vector<Jump>>;

#endif
