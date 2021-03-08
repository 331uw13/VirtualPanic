#include "math.hpp"


namespace vpanic {

	float angle_between(const Vec3& a, const Vec3& b) {
		return (a.x*b.x + a.y*b.y + a.z*b.z) / (a.length()*b.length());
	}
	
	float dot(const Vec3& a, const Vec3& b) {
		return (a.length()*b.length()) * angle_between(a, b);
	}

	Vec3 cross(const Vec3& a, const Vec3& b) {
		return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}

}
