#pragma once

#include "Grid2D.h"

namespace TinyCartographer {
class ProbabilityGrid : public Grid2D {
public:
	ProbabilityGrid(const MapLimits &map_limits, const std::shared_ptr <ValueConversionTables> &value_tables);

	void SetProbability(const Eigen::Vector2i &cell_index, const float prob);

	float GetProbability(const Eigen::Vector2i &cell_index);

private:
	std::shared_ptr <ValueConversionTables> value_tables_;
};
}  // namespace TinyCartographer