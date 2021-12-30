#include "SearchParameters.h"

#pragma once
#include <vector>
#include "PointCloud.h"

namespace TinyCartographer {

SearchParameters::SearchParameters(const double half_linear_w, const double half_ang_w, const PointCloud& point_cloud,
                                   const double resolution)
    : resolution_(resolution)
{
    // 找到最远的点
    float scan_range_max = 3 * resolution;
    for (size_t i = 0; i < point_cloud.points_.size(); ++i)
    {
        const float range = point_cloud.points_[i].norm();
        scan_range_max = std::max(scan_range_max, range);
    }

    // 根据最远点距离来计算角度步长，越远角度步长越小
    ang_step_ = (1 - 1e-3) * std::acos(1 - resolution_ * resolution_ / (2 * scan_range_max * scan_range_max));

    // 角度搜索空间数量
    half_ang_nums_ = std::ceil(half_ang_w / ang_step_);
    ang_nums_ = 2 * half_ang_nums_ + 1;

    // 位置搜索框
    const int half_linear_indexs_ = std::ceil(half_linear_w / resolution);
    linear_bounds_.reserve(ang_nums_);
    for (int i = 0; i != ang_nums_; ++i)
    {
        linear_bounds_.push_back(
            LinearBounds{-half_linear_indexs_, half_linear_indexs_, -half_linear_indexs_, half_linear_indexs_});
    }
}

std::vector<PointCloud> GenerateRotatedScans(const PointCloud& point_cloud, const SearchParameters& sp)
{
    std::vector<PointCloud> rotated_scans;
    rotated_scans.reserve(sp.ang_nums_);

    double delta_ang = -sp.half_ang_nums_ * sp.ang_step_;
    for (int i = 0; i < sp.ang_nums_; ++i, delta_ang += sp.ang_step_)
    {
        // todo：确认是否可行
        Transform2f rotation(Eigen::Vector2f::Zero(), delta_ang);
        rotated_scans.push_back(rotation * point_cloud);
    }

    return rotated_scans;
}

std::vector<std::vector<Eigen::Vector2i>> GenerateTranslatedScansAftRotated(
    std::vector<PointCloud>& all_rotated_pc, const Eigen::Translation<2, float>& translation,
    const MapLimits& map_limits)
{
}
}  // namespace TinyCartographer