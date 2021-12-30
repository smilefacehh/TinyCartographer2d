#include "PointCloud.h"

namespace TinyCartographer {
PointCloud operator*(const Transform2f& transform, const PointCloud& point_cloud)
{
    std::vector<Eigen::Vector2f> points;
    points.reserve(point_cloud.points_.size());
    for (int i = 0; i < point_cloud.points_.size(); ++i)
    {
        points.emplace_back(transform * point_cloud[i]);
    }
    return PointCloud(points, point_cloud.intensities_);
}
}  // namespace TinyCartographer