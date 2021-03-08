#include "math.hpp"


namespace vpanic {

	float to_radians(const float a) {
		return a*(M_PI/180.f);
	}

	float angle_between(const Vec3& a, const Vec3& b) {
		return (a.x*b.x + a.y*b.y + a.z*b.z) / (a.length()*b.length());
	}
	
	float dot(const Vec3& a, const Vec3& b) {
		Vec3 tmp = a * b;
		return tmp.x+tmp.y+tmp.z;
	}

	Vec3 cross(const Vec3& a, const Vec3& b) {	
		const float x = a.y*b.z-a.z*b.y;
		const float y = a.z*b.x-a.x*b.z;
		const float z = a.x*b.y-a.y*b.x;
		return Vec3(x, y, z);
	}

}
