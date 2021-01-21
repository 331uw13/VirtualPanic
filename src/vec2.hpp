#pragma once
#include <SDL2/SDL_rect.h>

namespace VPanic {

	struct vec2 : public SDL_FPoint {
		
		vec2(const float t_x, const float t_y) {
			x = t_x;
			y = t_y;
		}

		vec2(const float t_xy) {
			x = y = t_xy;
		}

		vec2() {
			x = y = 0;
		}

		~vec2() {};
		

		// rewrite all this ... i dont like it .. all of them are not needed
		
		bool   operator <    (const vec2& t_a0) const;
		bool   operator >    (const vec2& t_a0) const;
		bool   operator >=   (const vec2& t_a0) const;
		bool   operator <=   (const vec2& t_a0) const;
		bool   operator ==   (const vec2& t_a0) const;
		bool   operator !=   (const vec2& t_a0) const;
		
		vec2&  operator +=   (const vec2& t_a0);
		vec2&  operator -=   (const vec2& t_a0);
		vec2  operator +   (const vec2& t_a0);
		vec2  operator -   (const vec2& t_a0);
		vec2  operator /   (const float t_a0);
		vec2  operator +   (const float t_a0);
		vec2  operator -   (const float t_a0);
		
		float angle(const vec2& t_a0) const;
		float distance(const vec2& t_a0) const;
	};

	struct ivec2 : public SDL_Point {
		
		ivec2(const int t_x, const int t_y) {
			x = t_x;
			y = t_y;
		}

		ivec2(const int t_xy) {
			x = y = t_xy;
		}

		ivec2() {
			x = y = 0;
		}

		~vec2() {};

		//int angle(const ivec2& t_a0) const;
		//int distance(const ivec2& t_a0) const;

	};

}





