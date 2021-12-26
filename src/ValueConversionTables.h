#pragma once

#include <map>
#include <memory>
#include <vector>

namespace TinyCartographer {
/**
 * 概率值以uint16保存计算，本类提供概率转换
 */
class ValueConversionTables {
public:
	/**
	 * 根据传入的[l,u]将[1,32767]映射到[l,u]，0对应unknown_result，返回数组形式[unknown_result, l, ..., u,
	 * unknown_result, l, ..., u]，重复了一次
	 */
	const std::vector<float> *GetConversionTable(float unknown_result, float lower_bound, float upper_bound);

	/**
	 * [1,32767]->[l,u]，unknown_val->unknown_result
	 */
	static std::unique_ptr <std::vector<float>>
	BoundUint16ToFloat(const uint16_t unknown_val, const float unknown_result,
					   const float lower_bound, const float upper_bound);

private:

	std::map<const std::tuple<float, float, float>, std::unique_ptr<const std::vector<float>>> bounds_to_lookup_table_;
};
}  // namespace TinyCartographer