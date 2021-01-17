#pragma once

namespace VPanic {
	struct vec2 {
	
		float x;
		float y;

		vec2(const float t_x, const float t_y)
		   	: x(t_x), y(t_y) {}
		
		vec2(const float t_xy = 0)
		   	: x(t_xy), y(t_xy) {}
		
		~vec2() {};
		
		bool   operator <    (const vec2& t_a0) const;
		bool   operator >    (const vec2& t_a0) const;
		bool   operator >=   (const vec2& t_a0) const;
		bool   operator <=   (const vec2& t_a0) const;
		bool   operator ==   (const vec2& t_a0) const;
		bool   operator !=   (const vec2& t_a0) const;
		
		vec2&  operator +=   (const vec2& t_a0);
		vec2  operator +   (const vec2& t_a0);
		vec2  operator -   (const vec2& t_a0);
		vec2  operator /   (const float t_a0);
		vec2  operator +   (const float t_a0);
		vec2  operator -   (const float t_a0);
		
		float angle(const vec2& t_a0) const;
		float distance(const vec2& t_a0) const;
	};
}





