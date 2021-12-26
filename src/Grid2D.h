#pragma once

#include <memory>
#include "MapLimits.h"
#include "ValueConversionTables.h"

namespace TinyCartographer {
class Grid2D {
public:
	Grid2D(const MapLimits &map_limits, const float min_prob, const float max_prob,
		   std::shared_ptr <ValueConversionTables> &value_tables);

	const MapLimits &GetMapLimits();

	float GetProb(const Eigen::Vector2i &cell_index);

	float MinProb();

	float MaxProb();

	int ToFlatIndex(const Eigen::Vector2i &cell_index);

public:
	std::vector <uint16_t> cells_;

private:
	MapLimits map_limits_;
	float min_prob_;
	float max_prob_;
	const std::vector<float> *value_conversion_table_;
};
}  // namespace TinyCartographer