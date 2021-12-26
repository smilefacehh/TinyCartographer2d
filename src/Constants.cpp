#include "Constants.h"
#include "ValueConversionTables.h"

namespace TinyCartographer {
const std::vector<float> *kUint16ToFloatVec = ValueConversionTables::BoundUint16ToFloat(UNKNOWN_VAL, MAX_PROB,
																						MIN_PROB, MAX_PROB).release();
}