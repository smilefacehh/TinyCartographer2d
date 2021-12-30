#pragma once

#include "Grid2D.h"
#include "PointCloud.h"
#include "Transform.h"
#include "PrecomputationGrid2D.h"
#include "MapLimits.h"

namespace TinyCartographer {
class FastCorrelativeScanMatcher2D
{
public:
    FastCorrelativeScanMatcher2D(const Grid2D& grid, const int depth);
    ~FastCorrelativeScanMatcher2D();

    FastCorrelativeScanMatcher2D(const FastCorrelativeScanMatcher2D& rhs) = delete;
    FastCorrelativeScanMatcher2D& operator=(const FastCorrelativeScanMatcher2D& rhs) = delete;

    /**
     * 将点云与grid进行匹配，如果得分超过min_score，返回true，更新score、pose
     * @param initial_pose 初始化点云pose
     * @param point_cloud 点云
     * @param min_score 匹配得分阈值
     * @param score 实际匹配得分
     * @param pose 匹配pose
     * @return true表示匹配成功
     */
    bool Match(const Transform2f& initial_pose, const PointCloud& point_cloud, const float min_score, float& score,
               Transform2f& pose);

    /**
     * 同Match，只是这里是全部搜索空间
     */
    bool MatchFullSubmap(const PointCloud& point_cloud, const float min_score, float& score, Transform2f& pose);

private:
    bool MatchWithSearchParameter();

private:
    MapLimits map_limits_;
    std::unique_ptr<PrecomputationGridStack2D> precomputation_grid_stack_;
};
}  // namespace TinyCartographer
