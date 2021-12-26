#include "MapLimits.h"

namespace TinyCartographer {
MapLimits::MapLimits(const double resolution, const Eigen::Vector2i& n_cells)
    : resolution_(resolution), n_cells_(n_cells)
{
}

double MapLimits::Resolution() { return resolution_; }
const Eigen::Vector2i& MapLimits::NCells() { return n_cells_; }

Eigen::Vector2i MapLimits::ToIndex(const Eigen::Vector2f& point)
{
    return Eigen::Vector2i(round(point(0) / resolution_ + 0.5), round(point(1) / resolution_ + 0.5));
}

Eigen::Vector2f MapLimits::ToPoint(const Eigen::Vector2i& cell_index)
{
    return Eigen::Vector2f(resolution_ * (cell_index(0) + 0.5), resolution_ * (cell_index(1) + 0.5));
}

bool MapLimits::ValidIndex(const Eigen::Vector2i& cell_index)
{
    return (Eigen::Vector2i(0, 0) <= cell_index).all() && (cell_index < n_cells_).all();
}
}  // namespace TinyCartographer