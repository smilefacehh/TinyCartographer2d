#pragma once

#include "Grid2D.h"

namespace TinyCartographer {
class PrecomputationGrid2D {
public:
	/**
	 * w*w正方形滑，取滑窗中最大值作为当前cell的值
	 * @param grid 原始网格
	 * @param width 滑窗边长
	 * @param reusable_intermediate_grid 保存中间值
	 */
	PrecomputationGrid2D(const Grid2D &grid, const int width, std::vector<float> *reusable_intermediate_grid);

	/**
	 * 获取格子的值，[0,255]
	 * @param cell_index
	 * @return
	 */
	int GetValue(const Eigen::Vector2i &cell_index);

private:
	/**
	 * 概率转uint8
	 */
	uint8_t ProbToUint8(const float prob);

	const Eigen::Vector2i offset_;
	const Eigen::Vector2i wide_n_cells_;
	const float min_score_;
	const float max_score_;
	// [0,255]
	std::vector <uint8_t> cells_;
}

class PrecomputationGridStack2D {
public:
	PrecomputationGridStack2D(const Grid2D &grid, const int depth);

	const PrecomputationGrid2D &Get(int index);

	int MaxDepth();

private:
	std::vector <PrecomputationGrid2D> precomputation_grids_;
};
}