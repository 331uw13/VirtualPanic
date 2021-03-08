#pragma once
#include "vec2.hpp"

namespace vpanic {

	struct Vec3 {
		
		float x  { 0.0f };
		float y  { 0.0f };
		float z  { 0.0f };


		Vec3(const float t_xyz) 
			: x(t_xyz), y(t_xyz), z(t_xyz) {}
		
		Vec3(const float t_x, const float t_y, const float t_z) 
			: x(t_x), y(t_y), z(t_z) {}
		
		Vec3() {}

		
		float length() const;
		Vec3 invert() const;
		Vec3 normalize() const;
		void normalize_self();
		

		bool all(const float t) const;
		bool any(const float t) const;

		Vec2 xy() const { return Vec2(x, y); }

		Vec3  operator -  (const Vec3& a) const;
		Vec3  operator -  (const float a) const;
		Vec3& operator -= (const Vec3& a);
		Vec3& operator -= (const float a);
		Vec3  operator +  (const Vec3& a) const;
		Vec3  operator +  (const float a) const;
		Vec3& operator += (const Vec3& a);
		Vec3& operator += (const float a);
		Vec3  operator /  (const Vec3& a) const;
		Vec3  operator /  (const float a) const;
		Vec3& operator /= (const Vec3& a);
		Vec3& operator /= (const float a);
		Vec3  operator *  (const Vec3& a) const;
		Vec3  operator *  (const float a) const;
		Vec3& operator *= (const Vec3& a);
		Vec3& operator *= (const float a);
	
	};

}

