#pragma once
#include <cmath>

#include "vec3.hpp"
#include "matrix.hpp"


namespace vpanic {

	float to_radians(const float a);
	float angle_between(const Vec3& a, const Vec3& b);
	float dot(const Vec3& a, const Vec3& b);
	Vec3 cross(const Vec3& a, const Vec3& b);

}
