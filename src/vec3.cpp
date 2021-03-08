#include <cmath>
#include "vec3.hpp"
#include "math.hpp"

namespace vpanic {

	bool Vec3::all(const float t) const {
		return (x == t && y == t && z == t);
	}

	bool Vec3::any(const float t) const {
		return (x == t || y == t || z == t);
	}

	float Vec3::length() const {
		return sqrt(dot(*this, *this));
	}

	Vec3 Vec3::invert() const {
		return Vec3(-x, -y, -z);
	}
	
	Vec3 Vec3::normalize() const {
		const float l = length();
		return Vec3(x/l, y/l, z/l);
	}

	void Vec3::normalize_self() {
		const float l = length();
		x /= l;
		y /= l;
		z /= l;
	}

	Vec3 Vec3::operator - (const Vec3& a) const {
		return Vec3(x-a.x, y-a.y, z-a.z);
	}

	Vec3 Vec3::operator - (const float a) const {
		return Vec3(x-a, y-a, z-a);
	}
	
	Vec3& Vec3::operator -= (const Vec3& a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vec3& Vec3::operator -= (const float a) {
		x -= a;
		y -= a;
		z -= a;
		return *this;
	}

	Vec3 Vec3::operator + (const Vec3& a) const {
		return Vec3(x+a.x, y+a.y, z+a.z);
	}

	Vec3 Vec3::operator + (const float a) const {
		return Vec3(x+a, y+a, z+a);
	}

	Vec3& Vec3::operator += (const Vec3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	
	Vec3& Vec3::operator += (const float a) {
		x += a;
		y += a;
		z += a;
		return *this;
	}

	Vec3 Vec3::operator / (const Vec3& a) const {
		return Vec3(x/a.x, y/a.y, z/a.z);
	}
	
	Vec3 Vec3::operator / (const float a) const {
		return Vec3(x/a, y/a, z/a);
	}

	Vec3& Vec3::operator /= (const Vec3& a) {
		x /= a.x;
		y /= a.y;
		z /= a.z;
		return *this;
	}

	Vec3& Vec3::operator /= (const float a) {
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	Vec3 Vec3::operator * (const Vec3& a) const {
		return Vec3(x*a.x, y*a.y, z*a.z);
	}

	Vec3 Vec3::operator * (const float a) const {
		return Vec3(x*a, y*a, z*a);
	}

	Vec3& Vec3::operator *= (const Vec3& a) {
		x *= a.x;
		y *= a.y;
		z *= a.z;
		return *this;
	}

	Vec3& Vec3::operator *= (const float a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}


}
