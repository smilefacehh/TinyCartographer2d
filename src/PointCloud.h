#pragma once

#include <Eigen/Core>
#include <vector>
#include "Transform.h"

namespace TinyCartographer {
class PointCloud
{
public:
    using PointType = Eigen::Vector2f;

    PointCloud() {}

    explicit PointCloud(const std::vector<PointType>& points) : points_(points) {}

    PointCloud(const std::vector<PointType>& points, const std::vector<float>& intensities)
        : points_(points), intensities_(intensities)
    {
    }
    
public:
    std::vector<PointType> points_;
    std::vector<float> intensities_;
};

// 变换 * 点云
PointCloud operator*(const Transform2f& transform, const PointCloud& point_cloud);
}  // namespace TinyCartographer