#pragma once
#include "vec4.hpp"
#include "vec3.hpp"


namespace vpanic {

	class Matrix {
	public:

		Matrix(const float t);
		Matrix(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d);
		Matrix() {}
		
		Matrix   operator +  (const Matrix& a) const;
		Matrix&  operator += (const Matrix& a);
		Matrix   operator -  (const Matrix& a) const;
		Matrix&  operator -= (const Matrix& a);
		Matrix   operator *  (const Matrix& a) const;
		Matrix&  operator *= (const Matrix& a);

		Vec4     operator [] (const unsigned char i) const;
		Vec4&    operator [] (const unsigned char i);
	
		void translate(const Vec3& pos);
		void scale(const Vec3& vec);
		void rotate(const Vec3& r, const float angle);
		void copy_rotation(const Matrix& mat);
		void clear();
		float* begin() const;
	
	private:

		Vec4 m[4];

	};

	void get_projection(Matrix& t_mat, const float t_fov, const float t_aspect_ratio, const float t_znear, const float t_zfar);

}



