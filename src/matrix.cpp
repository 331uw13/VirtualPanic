#include <cmath>
#include <cassert>

#include "matrix.hpp"


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

	Matrix Matrix::operator + (const Matrix& a) {
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

	Matrix Matrix::operator - (const Matrix& a) {
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
	
	Matrix Matrix::operator * (const Matrix& a) {
		Matrix tmp(m[0], m[1], m[2], m[3]);
		for(int i = 0; i < 4; i++) {
			tmp[i] = Vec4( 
					tmp[i].x*a[0].x + tmp[i].y*a[1].x + tmp[i].z*a[2].x + tmp[i].w*a[3].x,
					tmp[i].x*a[0].y + tmp[i].y*a[1].y + tmp[i].z*a[2].y + tmp[i].w*a[3].y,
					tmp[i].x*a[0].z + tmp[i].y*a[1].z + tmp[i].z*a[2].z + tmp[i].w*a[3].z,
					tmp[i].x*a[0].w + tmp[i].y*a[1].w + tmp[i].z*a[2].w + tmp[i].w*a[3].w
					);
		}
		return tmp;
	}

	Matrix& Matrix::operator *= (const Matrix& a) {
		for(int i = 0; i < 4; i++) {
			m[i] = Vec4( 
					m[i].x*a[0].x + m[i].y*a[1].x + m[i].z*a[2].x + m[i].w*a[3].x,
					m[i].x*a[0].y + m[i].y*a[1].y + m[i].z*a[2].y + m[i].w*a[3].y,
					m[i].x*a[0].z + m[i].y*a[1].z + m[i].z*a[2].z + m[i].w*a[3].z,
					m[i].x*a[0].w + m[i].y*a[1].w + m[i].z*a[2].w + m[i].w*a[3].w
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

	void Matrix::scale(const Vec3& t_vec) {
		m[0].x *= t_vec.x;
		m[0].y *= t_vec.x;
		m[0].z *= t_vec.x;
		m[0].w *= t_vec.x;
		m[1].x *= t_vec.y;
		m[1].y *= t_vec.y;
		m[1].z *= t_vec.y;
		m[1].w *= t_vec.y;
		m[2].x *= t_vec.z;
		m[2].y *= t_vec.z;
		m[2].z *= t_vec.z;
		m[2].w *= t_vec.z;
	}

	void Matrix::rotate(const Vec3& t_r, const float t_angle) {		
		
		Matrix rm(m[0], m[1], m[2], m[3]);
		Matrix res(1.0f);
		
		// i have to admit that i got stuck here and had to check how they did it in glm
		// source: https://github.com/g-truc/glm/blob/64be09e1b1390ddcd0f79f38531855a9de26ad1f/glm/ext/matrix_transform.inl#L18
		
		const float c = cos(t_angle);
		const float s = sin(t_angle);
		const Vec3 n = t_r.normalize();
		Vec3 i(1.0f-c);
	
		i.x *= n.x;
		i.y *= n.y;
		i.z *= n.z;

		rm[0].x = c + i.x * n.x;
		rm[0].y = i.x * n.y + s * n.z;
		rm[0].z = i.x * n.z - s * n.y;
		
		rm[1].x = i.y * n.x - s * n.z;
		rm[1].y = c + i.y * n.y;
		rm[1].z = i.y * n.z + s * n.x;

		rm[2].x = i.z * n.x + s * n.y;
		rm[2].y = i.z * n.y - s * n.x;
		rm[2].z = c + i.z * n.z;

		for(int i = 0; i < 4; i++) {
			res[i] = Vec4( 
					m[0].x*rm[i].x + m[1].x*rm[i].y + m[2].x*rm[i].z,
					m[0].y*rm[i].x + m[1].y*rm[i].y + m[2].y*rm[i].z,
					m[0].z*rm[i].x + m[1].z*rm[i].y + m[2].z*rm[i].z,
					m[i].w
					);
		}

		m[0] = res[0];
		m[1] = res[1];
		m[2] = res[2];
	}
	
	float* Matrix::begin() const {
		return (float*)(&m[0].x);
	}


}
