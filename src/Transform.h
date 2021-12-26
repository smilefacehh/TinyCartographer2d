#pragma once

#include <Eigen/Core>
#include <string>
#include <sstream>
#include "Util.h"

namespace TinyCartographer {
class Transform2f {
public:
	Transform2f() : translation_(Eigen::Vector2f::Zero()), rotation_(Eigen::Rotation2D<float>::Identity()) {}

	Transform2f(const Eigen::Vector2f &translation, const Eigen::Rotation2D<float> &rotation) : translation_(
			translation), rotation_(rotation) {}

	Transform2f(const Eigen::Vector2f &translation, const double rotation) : translation_(
			translation), rotation_(rotation) {}

	const Eigen::Vector2f &Translation() { return translation_; }

	const Eigen::Rotation2D<float> Rotation() { return rotation_; }

	double NormalizedAngle() { return WrapToPi(rotation_.angle()); }

	Transform2f Inverse() {
		const Eigen::Rotation2D<float> r = rotation_.inverse();
		const Eigen::Vector2f t = -rotation_ * translation_;
		return Transform2f(t, r);
	}

	static Transform2f Rotation(const double rotation) {
		return Transform2f(Eigen::Vector2f::Zero(), rotation);
	}

	static Transform2f Rotation(const Eigen::Rotation2D<float> &rotation) {
		return Transform2f(Eigen::Vector2f::Zero(), rotation);
	}

	static Transform2f Translation(const Eigen::Vector2f &vec) {
		return Transform2f(vec, Eigen::Rotation2D<float>::Identity());
	}

	static Transform2f Identity() {
		return Transform2f();
	}

	std::string DebugString() {
		std::ostringstream ss;
		ss << "t:(" << translation_.x() << "," << translation_.y() << "), angle:" << rotation_.angle();
		return ss.str();
	}

private:
	Eigen::Vector2f translation_;
	Eigen::Rotation2D<float> rotation_;
};

// pose相乘
Transform2f operator*(const Transform2f &lhs, const Transform2f &rhs) {
	return Transform2f(lhs.Rotation() * rhs.Translation() + lhs.Translation(), lhs.Rotation() * rhs.Rotation());
}

// pose*点
Eigen::Vector2f operator*(const Transform2f &transform, const Eigen::Vector2f &point) {
	return transform.Rotation() * point + transform.Translation();
}
}