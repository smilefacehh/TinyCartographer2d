#pragma once

#include <Eigen/Core>
#include <vector>

namespace TinyCartographer {
class PointCloud {
public:
	using PointType = Eigen::Vector2f;

	PointCloud(){}

	explicit PointCloud(const std::vector <PointType> &points):points_(points){}

	PointCloud(const std::vector<PointType>& points, const std::vector < float> &intensities):points_(points), intensities_(intensities){}

	size_t Size() { return points_.size();}

public:
	std::vector<PointType> points_;
	std::vector<float> intensities_;
};
}