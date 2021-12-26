#pragma once

#include "Grid2D.h"

namespace TinyCartographer {
class FastCorrelativeScanMatcher2D {
public:
	FastCorrelativeScanMatcher2D(const Grid2D &grid, const int depth);

	~FastCorrelativeScanMatcher2D();

	FastCorrelativeScanMatcher2D(const FastCorrelativeScanMatcher2D &rhs) = delete;

	FastCorrelativeScanMatcher2D &operator=(const FastCorrelativeScanMatcher2D &rhs) = delete;

	bool Match();
};
}
