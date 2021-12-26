#pragma once

#include <Eigen/Core>

namespace TinyCartographer {
class MapLimits
{
public:
    /**
     * 地图尺寸
     * @param resolution 分辨率
     * @param n_cells x、y方向网格数量
     */
    MapLimits(const double resolution, const Eigen::Vector2i& n_cells);

    double Resolution();
    const Eigen::Vector2i& NCells();

    /**
     * 坐标点转网格坐标
     * 两个坐标重合
     * @param point 坐标点
     * @return 网格坐标
     */
    Eigen::Vector2i ToIndex(const Eigen::Vector2f& point);

    /**
     * 网格坐标转坐标点
     * @param cell_index 网格坐标
     * @return 坐标点
     */
    Eigen::Vector2f ToPoint(const Eigen::Vector2i& cell_index);

    /**
     * 网格坐标判断
     * @param cell_index 网格坐标
     * @return true表示坐标合法
     */
    bool ValidIndex(const Eigen::Vector2i& cell_index);

private:
    double resolution_;
    Eigen::Vector2i n_cells_;
};
}  // namespace TinyCartographer