#pragma

#include <stdint.h>

namespace TinyCartographer {

template<typename T>
T Clamp(const T val, const T lower_bound, const T upper_bound) {
	if (val < lower_bound) {
		return lower_bound;
	} else if (val > upper_bound) {
		return upper_bound;
	}

	return val;
}

/**
 * 概率->[1,32767]
 */
uint16_t ProbToUint16(const float prob, const float lower_bound, const float upper_bound);

/**
 * [1,32767]->概率
 */
float Uint16ToProb(const uint16_t val, const uint16_t unknown_val, const float unknow_result, const float lower_bound,
				   const float upper_bound);

/**
 * [1,65536]->概率，计算一次
 */
float Uint16ToProb(const uint16_t val);

/**
 * 转换到[-pi,pi]
 */
float WrapToPi(float rad);
}