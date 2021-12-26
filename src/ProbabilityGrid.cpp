#include "ProbabilityGrid.h"
#include "Constants.h"
#include "Util.h"

namespace TinyCartographer {
ProbabilityGrid::ProbabilityGrid(const MapLimits &map_limits,
								 const std::shared_ptr <ValueConversionTables> &value_tables)
		: Grid2D(map_limits, MIN_PROB, MAX_PROB, value_tables), value_tables_(value_tables) {}

void ProbabilityGrid::SetProbability(const Eigen::Vector2i &cell_index, const float prob) {
	cells_[ToFlatIndex(cell_index)] = ProbToUint16(prob);
}

float ProbabilityGrid::ProbabilityGrid::GetProbability(const Eigen::Vector2i &cell_index) {
	if (!GetMapLimits().ValidIndex(cell_index)) {
		return MIN_PROB;
	}
	return Uint16ToProb(cells_[ToFlatIndex(cell_index)]);
}
}