#pragma once

#include <iterator>
#include <Eigen/Core>

namespace TinyCartographer {
class XYIndexRangeIterator : public std::iterator<std::input_iterator_tag, Eigen::Vector2i> {
public:
	XYIndexRangeIterator(const Eigen::Vector2i &min_xy_index, const Eigen::Vector2i &max_xy_index)
			: max_xy_index_(min_xy_index), max_xy_index_(max_xy_index), xy_index_(min_xy_index) {}

	explicit XYIndexRangeIterator(const Eigen::Vector2i &n_cells) :
			XYIndexRangeIterator(Eigen::Vector2i::Zero(), Eigen::Vector2i(n_cells(0) - 1, n_cells(1) - 1)) {}

	XYIndexRangeIterator &operator++() {
		if (xy_index_(0) < max_xy_index_(0)) {
			++xy_index_(0);
		} else {
			xy_index_(0) = min_xy_index_(0);
			++xy_index_(1);
		}
		return *this;
	}

	Eigen::Vector2i &operator*() { return xy_index_; }

	bool operator==(const XYIndexRangeIterator &other) {
		return (xy_index_ == other.xy_index_).all();
	}

	bool operator!=(const XYIndexRangeIterator &other) {
		return !operator==(other);
	}

	XYIndexRangeIterator begin() {
		return XYIndexRangeIterator(min_xy_index_, max_xy_index_);
	}

	XYIndexRangeIterator end() {
		XYIndexRangeIterator it = begin();
		it.xy_index_ = Eigen::Vector2i(min_xy_index_(0), max_xy_index_(1) + 1);
		return it;
	}

private:
	Eigen::Vector2i min_xy_index_;
	Eigen::Vector2i max_xy_index_;
	Eigen::Vector2i xy_index_;
};
}