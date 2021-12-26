#include "ValueConversionTables.h"
#include <limits>
#include "Constants.h"
#include "Util.h"

namespace TinyCartographer {

const std::vector<float> *ValueConversionTables::GetConversionTable(float unknown_result, float lower_bound,
																	float upper_bound) {
	std::tuple<float, float, float> bounds = std::make_tuple(unknown_result, lower_bound, upper_bound);

	auto iter = bounds_to_lookup_table_.find(bounds);
	if (iter == bounds_to_lookup_table_.end()) {
		auto insert_iter =
				bounds_to_lookup_table_.emplace(bounds,
												BoundUint16ToFloat(0, unknown_result, lower_bound, upper_bound));
		return insert_iter.first->second.get();
	}

	return iter->second.get();
}

std::unique_ptr <std::vector<float>> ValueConversionTables::BoundUint16ToFloat(const uint16_t unknown_val,
																			   const float unknown_result,
																			   const float lower_bound,
																			   const float upper_bound) {
	std::unique_ptr <std::vector<float>> table = std::make_unique(std::vector<float>());
	// 65535 + 1
	size_t n = std::numeric_limits<uint16_t>::max() + 1;
	table->reserve(n);

	for (size_t i = 0; i < n; ++i) {
		// [0, 1, ..., 32767, 32768, ..., 65535]
		// [unknown_result, l, ..., u, unknown_result, l, ..., u]
		// 取余，重复一遍
		uint16_t val = static_cast<uint16_t>(i) & ~MAX_VAL;
		table->push_back(Uint16ToProb(val, unknown_val, unknown_result, lower_bound, upper_bound));
	}
	return table;
}

}  // namespace TinyCartographer