#include "PrecomputationGrid2D.h"
#include <deque>

namespace TinyCartographer {
/**
 * 队列中的元素始终保持非递增顺序
 */
class SlidingWindowMaximum {
public:
	void PushBack(const float val) {
		while (!non_ascending_vals_.empty() && val > non_ascending_vals_.back()) {
			non_ascending_vals_.pop_back();
		}
		non_ascending_vals_.push_back(val);
	}

	void PopFront(const float val) {
		if (!non_ascending_vals_.empty() && val == non_ascending_vals_.front()) {
			non_ascending_vals_.pop_front();
		}
	}

	void GetMax() {
		return non_ascending_vals_.empty() ? 0 : non_ascending_vals_.front();
	}

	bool Empty() {
		return non_ascending_vals_.empty();
	}

private:
	std::deque<float> non_ascending_vals_;
};

PrecomputationGrid2D::PrecomputationGrid2D(const Grid2D &grid, const int width,
										   std::vector<float> *reusable_intermediate_grid)
		: offset_(-width + 1, -width + 1),
		  wide_n_cells_(grid.GetMapLimits().NCells()(0) - offset_(0), grid.GetMapLimits().NCells()(1) - offset_(1)),
		  min_score_(grid.MinProb()), max_score_(grid.MaxProb()), cells_(wide_n_cells_(0) * wide_n_cells_(1)) {

	const int n_cells_x = grid.GetMapLimits().NCells()(0);
	const int n_cells_y = grid.GetMapLimits().NCells()(1);
	const int stride = wide_n_cells_(0);

	// 按行顺序保存成一维数组，存概率
	std::vector<float> &intermidiate = *reusable_intermediate_grid;
	intermidiate.resize(wide_n_cells_(0) * n_cells_y);

	// 按行遍历，intermidiate每个格子存前面w个格子的最大值，最后往外扩展w个格子
	for (int y = 0; y != n_cells_y; ++y) {
		// 每行重新开始
		SlidingWindowMaximum current_vals;
		// 行首元素
		current_vals.PushBack(std::abs(grid.GetProb(Eigen::Vector2i(0, y))));
		// 遍历行首w个格子
		for (int x = -width + 1; x < 0; ++x) {
			// w个格子依次存入当前行最大值
			intermidiate[x + width - 1 + y * stride] = current_vals.GetMax();
			if (x + width < n_cells_x) {
				// 行队列存入下一个格子的值
				current_vals.PushBack(std::abs(grid.GetProb(Eigen::Vector2i(x + width, y))));
			}
		}
		// 遍历剩下所有格子，w+1开始
		for (int x = 0; x < n_cells_x - width; ++x) {
			// 存下当前行最大值
			intermidiate[x + width - 1 + y * stride] = current_vals.GetMax();
			// 如果往前第w个格子是当前队列最大值，那么删掉，如果不是，表明最大值是在最近w个格子中某个
			current_vals.PopFront(std::abs(grid.GetProb(Eigen::Vector2i(x, y))));
			// 行队列存入下一个格子的值
			current_vals.PushBack(std::abs(grid.GetProb(Eigen::Vector2i(x + width, y))));
		}
		// 行尾加上w个格子
		for (int x = std::max(n_cells_x - width, 0); x < n_cells_x; ++x) {
			// 存下当前行最大值
			intermidiate[x + width - 1 + y * stride] = current_vals.GetMax();
			// 如果往前第w个格子是当前队列最大值，那么删掉，如果不是，表明最大值是在最近w个格子中某个
			current_vals.PopFront(std::abs(grid.GetProb(Eigen::Vector2i(x, y))));
		}
	}

	// 按列遍历，实际上先按行遍历再按列遍历，更新cell值，是cell左上角w*w的块的最大值
	for (int x = 0; x != wide_n_cells_(0); ++x) {
		// 每列重新开始
		SlidingWindowMaximum current_vals;
		// 队列存入列首值
		current_vals.PushBack(intermidiate[x]);

		// 遍历前w个格子
		for (int y = -width + 1; y != 0; ++y) {
			// 取当前最大值，概率转uint8，保存
			cells_[x + (y + width - 1) * stride] = ProbToUint8(current_vals.GetMax());
			if (y + width < n_cells_y) {
				current_vals.PushBack(intermidiate[x + (y + width) * stride]);
			}
		}
		for (int y = 0; y < n_cells_y - width; ++y) {
			cells_[x + (y + width - 1) * stride] = ProbToUint8(current_vals.GetMax());
			current_vals.PopFront(intermidiate[x + y * stride]);
			current_vals.PushBack(intermidiate[x + (y + width) * stride]);
		}
		for (int y = std::max(n_cells_y - width, 0); y != n_cells_y; ++y) {
			cells_[x + (y + width - 1) * stride] = ProbToUint8(current_vals.GetMax());
			current_vals.PopFront(intermidiate[x + y * stride]);
		}
	}
}

int PrecomputationGrid2D::GetValue(const Eigen::Vector2i &cell_index) {
	const Eigen::Vector2i local_cell_index = cell_index - offset_;
	if (local_cell_index(0) >= wide_n_cells_(0) || local_cell_index(1) >= wide_n_cells_(1)) {
		return 0;
	}
	return cells_[local_cell_index(0) + local_cell_index(1) * wide_n_cells_(0)];
}

uint8_t PrecomputationGrid2D::ProbToUint8(const float prob) {
	uint8_t val = round((prob - min_score_) * (255.0 / (max_score_ - min_score_)));
	return val;
}

PrecomputationGridStack2D::PrecomputationGridStack2D(const Grid2D &grid, const int depth) {
	const int max_width = 1 << (depth - 1);
	precomputation_grids_.reserve(depth);
	std::vector<float> reusable_intermediate_grid;
	// 尺寸是最大的那个
	reusable_intermediate_grid.reserve(
			(grid.GetMapLimits().NCells()(0) + max_width - 1) * grid.GetMapLimits().NCells()(1));
	for (int i = 0; i < depth; ++i) {
		precomputation_grids_.emplace_back(grid, 1 << i, &reusable_intermediate_grid);
	}
}

const PrecomputationGrid2D &PrecomputationGridStack2D::Get(int index) { return precomputation_grids_[index]; }

int PrecomputationGridStack2D::MaxDepth() { return precomputation_grids_.size() - 1; }
}