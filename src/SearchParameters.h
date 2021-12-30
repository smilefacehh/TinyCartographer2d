#pragma once
#include <vector>
#include "MapLimits.h"
#include "PointCloud.h"

namespace TinyCartographer {

/**
 * 位置搜索框
 * 相对于初始化pose网格索引的偏移量
 */
struct LinearBounds
{
    int min_x_index_offset_;
    int max_x_index_offset_;
    int min_y_index_offset_;
    int max_y_index_offset_;
};

/**
 * 搜索参数
 */
struct SearchParameters
{
    /**
     * 构造
     * @param linear_w 一半位置搜索范围
     * @param ang_w 一半角度搜索范围
     * @param point_cloud 点云
     * @param resolution 分辨率
     */
    SearchParameters(const double half_linear_w, const double half_ang_w, const PointCloud& point_cloud,
                     const double resolution);

    // 角度搜索数量（一半）
    int half_ang_nums_;
    // 角度搜索步长
    double ang_step_;
    // 分辨率
    double resolution_;
    // 角度搜索数量
    int ang_nums_;
    // 每个角度对应的位置搜索框，实际上是一样的
    std::vector<LinearBounds> linear_bounds_;
};

/**
 * 候选解
 */
struct Candidate2D
{
    Candidate2D(const int ang_index, const int x_index_offset, const int y_index_offset, const SearchParameters& sp)
        : ang_index_(ang_index),
          x_index_offset_(x_index_offset),
          y_index_offset_(y_index_offset),
          x_offset_(-y_index_offset * sp.resolution_),
          y_offset_(-x_index_offset * sp.resolution_),
          ang_offset_((ang_index - sp.half_ang_nums_) * sp.ang_step_)

    {
    }

    // 候选解对应的角度索引
    int ang_index_;
    // 候选解网格坐标相对于初始pose网格坐标的偏移量
    int x_index_offset_;
    int y_index_offset_;
    // 候选解相对于初始pose的真实距离、角度偏移量
    double x_offset_;
    double y_offset_;
    double ang_offset_;
    // 候选解得分
    float score_;

    bool operator<(const Candidate2D& other) { return score_ < other.score_; }
    bool operator>(const Candidate2D& other) { return score_ > other.score_; }
};

/**
 * 初始点云经过旋转空间变换，得到所有旋转的点云数组
 * @param point_cloud 点云
 * @param sp 搜索空间参数
 * @return 旋转点云集合
 */
std::vector<PointCloud> GenerateRotatedScans(const PointCloud& point_cloud, const SearchParameters& sp);

/**
 * 将旋转后的所有点云，施加初始平移变换，保存网格坐标
 * @param all_rotated_pc 所有经过旋转变换的点云集合
 * @param translation 平移变换
 * @param map_limits 地图尺寸
 * @return 点云集合
 */
std::vector<std::vector<Eigen::Vector2i>> GenerateTranslatedScansAftRotated(
    std::vector<PointCloud>& all_rotated_pc, const Eigen::Translation<2, float>& translation,
    const MapLimits& map_limits);
}  // namespace TinyCartographer