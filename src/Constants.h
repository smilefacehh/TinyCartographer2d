#pragma once

#include <vector>

namespace TinyCartographer {
#define MAX_VAL 32768
#define UNKNOWN_VAL  0

#define UNKNOWN_PROB 0.0
#define MIN_PROB 0.1
#define MAX_PROB 0.9

// [1,65536]->[0.1,0.9] 常驻，避免多次计算
extern const std::vector<float> *kUint16ToFloatVec;
}  // namespace TinyCartographer