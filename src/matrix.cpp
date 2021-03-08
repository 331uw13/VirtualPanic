//#include <cmath>
#include <cassert>

#include "matrix.hpp"
#include "math.hpp"

namespace vpanic {


	Matrix::Matrix(const float t) {
		m[0] = Vec4(t,    0.0f, 0.0f, 0.0f);
		m[1] = Vec4(0.0f, t,    0.0f, 0.0f);
		m[2] = Vec4(0.0f, 0.0f, t,    0.0f);
		m[3] = Vec4(0.0f, 0.0f, 0.0f, t   );
	}
		
	Matrix::Matrix(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d) {
		m[0] = a;
		m[1] = b;
		m[2] = c;
		m[3] = d;
	}

	Matrix Matrix::operator + (const Matrix& a) const {
		return Matrix(
				Vec4( m[0].x+a[0].x, m[0].y+a[0].y, m[0].z+a[0].z, m[0].w+a[0].w ),
				Vec4( m[1].x+a[1].x, m[1].y+a[1].y, m[1].z+a[1].z, m[1].w+a[1].w ),
				Vec4( m[2].x+a[2].x, m[2].y+a[2].y, m[2].z+a[2].z, m[2].w+a[2].w ),
				Vec4( m[3].x+a[3].x, m[3].y+a[3].y, m[3].z+a[3].z, m[3].w+a[3].w )
				);
	}

	Matrix& Matrix::operator += (const Matrix& a) {
		for(int i = 0; i < 4; i++) {
			m[i].x += a[i].x;
			m[i].y += a[i].y;
			m[i].z += a[i].z;
			m[i].w += a[i].w;
		}
		return *this;
	}

	Matrix Matrix::operator - (const Matrix& a) const {
		return Matrix(
				Vec4( m[0].x-a[0].x, m[0].y-a[0].y, m[0].z-a[0].z, m[0].w-a[0].w ),
				Vec4( m[1].x-a[1].x, m[1].y-a[1].y, m[1].z-a[1].z, m[1].w-a[1].w ),
				Vec4( m[2].x-a[2].x, m[2].y-a[2].y, m[2].z-a[2].z, m[2].w-a[2].w ),
				Vec4( m[3].x-a[3].x, m[3].y-a[3].y, m[3].z-a[3].z, m[3].w-a[3].w )
				);
	}

	Matrix& Matrix::operator -= (const Matrix& a) {
		for(int i = 0; i < 4; i++) {
			m[i].x -= a[i].x;
			m[i].y -= a[i].y;
			m[i].z -= a[i].z;
			m[i].w -= a[i].w;
		}
		return *this;
	}
	
	Matrix Matrix::operator * (const Matrix& a) const {
		Matrix tmp;
		for(int i = 0; i < 4; i++) {
			tmp[i] = Vec4(
					m[0].x*a[i].x + m[1].x*a[i].y + m[2].x*a[i].z + m[3].x*a[i].w,
					m[0].y*a[i].x + m[1].y*a[i].y + m[2].y*a[i].z + m[3].y*a[i].w,
					m[0].z*a[i].x + m[1].z*a[i].y + m[2].z*a[i].z + m[3].z*a[i].w,
					m[0].w*a[i].x + m[1].w*a[i].y + m[2].w*a[i].z + m[3].w*a[i].w
					);
		}
		return tmp;
	}

	Matrix& Matrix::operator *= (const Matrix& a) {
		for(int i = 0; i < 4; i++) {
			m[i] = Vec4( 
					m[0].x*a[i].x + m[1].y*a[i].x + m[2].z*a[i].x + m[3].w*a[i].x,
					m[0].x*a[i].y + m[1].y*a[i].y + m[2].z*a[i].y + m[3].w*a[i].y,
					m[0].x*a[i].z + m[1].y*a[i].z + m[2].z*a[i].z + m[3].w*a[i].z,
					m[0].x*a[i].w + m[1].y*a[i].w + m[2].z*a[i].w + m[3].w*a[i].w
					);
		}
		return *this;
	}

	Vec4 Matrix::operator [] (const unsigned char i) const {
		assert(i <= 3);
		return m[i];
	}

	Vec4& Matrix::operator [] (const unsigned char i) {
		assert(i <= 3);
		return m[i];
	}

	// -----------------------------

	void Matrix::translate(const Vec3& pos) {
		m[3] = Vec4(
				m[0].x*pos.x + m[1].x*pos.y + m[2].x*pos.z + m[3].x,
				m[0].y*pos.x + m[1].y*pos.y + m[2].y*pos.z + m[3].y,
				m[0].z*pos.x + m[1].z*pos.y + m[2].z*pos.z + m[3].z,
				m[0].w*pos.x + m[1].w*pos.y + m[2].w*pos.z + m[3].w
				);
	}

	void Matrix::scale(const Vec3& vec) {
		m[0].x *= vec.x;
		m[0].y *= vec.x;
		m[0].z *= vec.x;
		m[0].w *= vec.x;
		m[1].x *= vec.y;
		m[1].y *= vec.y;
		m[1].z *= vec.y;
		m[1].w *= vec.y;
		m[2].x *= vec.z;
		m[2].y *= vec.z;
		m[2].z *= vec.z;
		m[2].w *= vec.z;
	}

	void Matrix::rotate(const Vec3& r, const float angle) {		
		
		Matrix rm(m[0], m[1], m[2], m[3]);
		Matrix res(1.0f);
		
		// i have to admit that i got stuck here and had to check how they did it in glm
		// source: https://github.com/g-truc/glm/blob/64be09e1b1390ddcd0f79f38531855a9de26ad1f/glm/ext/matrix_transform.inl#L18
		
		const float a = to_radians(angle);
		const float c = cos(a);
		const float s = sin(a);
		const Vec3 n = r.normalize();
		Vec3 i(1.0f-c);
		i *= n;

		rm[0].x = c + i.x * n.x;
		rm[0].y = i.x * n.y + s * n.z;
		rm[0].z = i.x * n.z - s * n.y;
		
		rm[1].x = i.y * n.x - s * n.z;
		rm[1].y = c + i.y * n.y;
		rm[1].z = i.y * n.z + s * n.x;

		rm[2].x = i.z * n.x + s * n.y;
		rm[2].y = i.z * n.y - s * n.x;
		rm[2].z = c + i.z * n.z;

		Vec4 last = m[3];

		for(int i = 0; i < 4; i++) {
			res[i] = Vec4( 
					m[0].x*rm[i].x + m[1].x*rm[i].y + m[2].x*rm[i].z,
					m[0].y*rm[i].x + m[1].y*rm[i].y + m[2].y*rm[i].z,
					m[0].z*rm[i].x + m[1].z*rm[i].y + m[2].z*rm[i].z,
					m[0].w*rm[i].x + m[1].w*rm[i].y + m[2].w*rm[i].z
					);
		}
	
		*this = res;
		m[3] = last;
	}

	float* Matrix::begin() const {
		return (float*)&m[0].x;
	}
	
	// ---------------------------------

	void get_projection(Matrix& t_mat, const float t_fov, const float t_aspect_ratio, const float t_znear, const float t_zfar) {
		
		// https://www.youtube.com/watch?v=ih20l3pJoeU (16:00 - 28:20)  javidx9 explained this very well, thanks!

		const float f = tan(to_radians(t_fov)/2.0f);
		const float zd = t_zfar-t_znear;

		Matrix p(1.0f);
		p[0].x = 1.0f/(f*t_aspect_ratio);
		p[1].y = 1.0f/f;
		p[2].z = -(t_zfar+t_znear)/zd;
		p[3].z = -(2.0f*t_zfar*t_znear)/zd;
		p[2].w = -1.0f;
		p[3].w = 0.0f;

		t_mat = p;
	}



}





