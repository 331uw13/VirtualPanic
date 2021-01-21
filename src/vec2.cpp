#include <math.h>
#include "vec2.hpp"

namespace VPanic {
	
	vec2& vec2::operator += (const vec2& t_a0) {
		this->x += t_a0.x;
		this->y += t_a0.y;
		return *this;
	}
	
	vec2& vec2::operator -= (const vec2& t_a0) {
		this->x -= t_a0.x;
		this->y -= t_a0.y;
		return *this;
	}

	vec2 vec2::operator + (const vec2& t_a0) {
		return vec2(this->x + t_a0.x, this->y + t_a0.y);
	}
		
	vec2 vec2::operator / (const float t_a0) {
		if(t_a0 == 0) {
			return vec2(this->x, this->y);
		}
		return vec2(this->x / t_a0, this->y / t_a0);
	}

	vec2 vec2::operator + (const float t_a0) {
		return vec2(this->x + t_a0, this->y + t_a0);
	}

	vec2 vec2::operator - (const float t_a0) {
		return vec2(this->x - t_a0, this->y - t_a0);
	}
	
	vec2 vec2::operator - (const vec2& t_a0) {
		return vec2(this->x - t_a0.x, this->y - t_a0.x);
	}

	bool vec2::operator < (const vec2& t_a0) const { return (this->x < t_a0.x && this->y < t_a0.y); }
	bool vec2::operator > (const vec2& t_a0) const { return (this->x > t_a0.x && this->y > t_a0.y); }
	bool vec2::operator <= (const vec2& t_a0) const { return (this->x <= t_a0.x && this->y <= t_a0.y); }	
	bool vec2::operator >= (const vec2& t_a0) const { return (this->x >= t_a0.x && this->y >= t_a0.y); }
	bool vec2::operator == (const vec2& t_a0) const { return (this->x == t_a0.x && this->y == t_a0.y); }
	bool vec2::operator != (const vec2& t_a0) const { return (this->x != t_a0.x && this->y != t_a0.y); }

	float vec2::angle(const vec2& t_a0) const {
		return atan2(t_a0.x - x, t_a0.y - y);
	}

	float vec2::distance(const vec2& t_a0) const {
		float dx = t_a0.x - x;
		float dy = t_a0.y - y;
		return sqrt(dx * dx + dy * dy);
	}
	
}
