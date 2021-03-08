#pragma once
#include "vec4.hpp"
#include "vec3.hpp"


namespace vpanic {

	class Matrix {
	public:

		Matrix(const float t);
		Matrix(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d);
		Matrix() {}
		
		Matrix   operator +  (const Matrix& a);
		Matrix&  operator += (const Matrix& a);
		Matrix   operator -  (const Matrix& a);
		Matrix&  operator -= (const Matrix& a);
		Matrix   operator *  (const Matrix& a);
		Matrix&  operator *= (const Matrix& a);

		Vec4     operator [] (const unsigned char i) const;
		Vec4&    operator [] (const unsigned char i);

		float* begin() const;	
		void translate(const Vec3& t_pos);
		void scale(const Vec3& t_vec);
		void rotate(const Vec3& t_r, const float t_angle);

	private:

		Vec4 m[4];

	};

}

