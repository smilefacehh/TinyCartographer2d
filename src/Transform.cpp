#include "Transform.h"

namespace TinyCartographer {
Transform2f operator*(const Transform2f& lhs, const Transform2f& rhs)
{
    return Transform2f(lhs.Rotation() * rhs.Translation() + lhs.Translation(), lhs.Rotation() * rhs.Rotation());
}

Eigen::Vector2f operator*(const Transform2f& transform, const Eigen::Vector2f& point)
{
    return transform.Rotation() * point + transform.Translation();
}
}  // namespace TinyCartographer