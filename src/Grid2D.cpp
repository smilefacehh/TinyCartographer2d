#include "Grid2D.h"
#include "Constants.h"

namespace TinyCartographer {

Grid2D::Grid2D(const MapLimits &map_limits, const float min_prob, const float max_prob,
			   std::shared_ptr <ValueConversionTables> &value_tables)
		: map_limits_(map_limits),
		  cells_(map_limits_.NCells()(0) * map_limits_.NCells()(1), UNKNOWN_PROB),
		  min_prob_(min_prob),
		  max_prob_(max_prob),
		  value_conversion_table_(value_tables->GetConversionTable(max_prob, min_prob, max_prob)) {
}

const MapLimits &Grid2D::GetMapLimits() { return map_limits_; }

float Grid2D::GetProb(const Eigen::Vector2i &cell_index) {
	if (!map_limits_.ValidIndex(cell_index)) {
		return MAX_PROB;
	}

	return (*value_conversion_table_)[cells_[ToFlatIndex(cell_index)]];
}

float Grid2D::MinProb() { return min_prob_; }

float Grid2D::MaxProb() { return max_prob_; }

int Grid2D::ToFlatIndex(const Eigen::Vector2i &cell_index) {
	return cell_index(0) + cell_index(1) * map_limits_.NCells(0);
}
}  // namespace TinyCartographer