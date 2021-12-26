#include "Util.h"
#include <math.h>

#include "Constants.h"

namespace TinyCartographer {
uint16_t ProbToUint16(const float prob, const float lower_bound, const float upper_bound) {
	return round((Clamp(prob, lower_bound, upper_bound) - lower_bound) * (MAX_VAL - 2.0) / (upper_bound - lower_bound));
}

float Uint16ToProb(const uint16_t val, const uint16_t unknown_val, const float unknow_result, const float lower_bound,
				   const float upper_bound) {
	if (val == unknown_val) {
		return unknow_result;
	}

	float k = (upper_bound - lower_bound) / (MAX_VAL - 2.0);
	return k * (val - 1) + lower_bound;
}

float Uint16ToProb(const uint16_t val) {
	return *kUint16ToFloatVec[val];
}

float WrapToPi(float rad) {
	while (rad > M_PI) rad -= 2. * M_PI;
	while (rad < -M_PI) rad += 2. * M_PI;
	return rad;
}
}