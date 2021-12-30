#include "VoxelFilter.h"
#include <random>
#include <unordered_map>

namespace TinyCartographer {
VoxelFilter::VoxelFilter(const float resolution) : resolution_(resolution) {}
VoxelFilter::~VoxelFilter() {}

PointCloud VoxelFilter::Filter(const PointCloud& point_cloud)
{
    const std::vector<bool> random_indices = RandomizedIndices(point_cloud, resolution_);
    std::vector<Eigen::Vector2f> points, intensities;
    for (size_t i = 0; i < point_cloud.points_.size(); ++i)
    {
        if (random_indices(i))
        {
            points.push_back(point_cloud.points_[i]);
        }
    }
    for (size_t i = 0; i < point_cloud.intensities_.size(); ++i)
    {
        if (random_indices(i))
        {
            intensities.push_back(point_cloud.intensities_[i]);
        }
    }
    return PointCloud(std::move(points), std::move(intensities));
}

VoxelKeyType VoxelFilter::VoxelIndex(const Eigen::Vector2f& point, const float resolution)
{
    // todo@lutao：把int类型赋值给uint类型，转换的索引是否有冲突
    const Eigen::Vector2f index = point / resolution;
    const uint64_t x = std::round(index.x());
    const uint64_t y = std::round(index.y());
    return (x << 32) + y;
}

std::vector<bool> VoxelFilter::RandomizedIndices(const PointCloud& point_cloud, const float resolution)
{
    std::minstd_rand0 gen;
    // 第一个int是落入voxel的点数量，第二个int是随机选取的点的idx
    std::unordered_map<VoxelKeyType, std::pair<int, int>> voxel_cnt_index;
    for (size_t i = 0; i < point_cloud.points_.size(); ++i)
    {
        VoxelKeyType key = VoxelIndex(point_cloud.points_[i], resolution);
        if (voxel_cnt_index.find(key) == voxel_cnt_index.end())
        {
            voxel_cnt_index.insert(std::make_pair(1, i));
        }
        else
        {
            // 每次voxel新加入一个点时，有1/n的概率替换成当前点的idx
            std::uniform_int_distribution<> distribution(1, voxel_cnt_index[key].first);
            if (distribution(gen) == voxel_cnt_index[key].first)
            {
                voxel_cnt_index[key].second = i;
            }
        }
    }

    std::vector<bool> random_indices(point_cloud.points_.size(), false);
    for (const auto& vo : voxel_cnt_index)
    {
        random_indices[vo.second.second] = true;
    }

    return random_indices;
}
}  // namespace TinyCartographer