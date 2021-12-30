#pragma once
#include "PointCloud.h"

namespace TinyCartographer {
class VoxelFilter
{
public:
    using VoxelKeyType = uint64_t;

    VoxelFilter(const float resolution);
    ~VoxelFilter();

    /**
     * 点云栅格化降采样
     * @param point_cloud 点云
     * @return 降采样后的点云
     */
    PointCloud Filter(const PointCloud& point_cloud);

private:
    /**
     * 计算点的栅格化坐标
     * @param point 点坐标
     * @param resolution 分辨率
     * @return 栅格索引
     */
    VoxelKeyType VoxelIndex(const Eigen::Vector2f& point, const float resolution);

    /**
     * 落入同一个栅格中的点随机保留其中一个
     * @param point_cloud 点云
     * @param resolution 分辨率
     * @return 保留的点对应为true
     */
    std::vector<bool> RandomizedIndices(const PointCloud& point_cloud, const float resolution);

private:
    float resolution_;
};
}  // namespace TinyCartographer