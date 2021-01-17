#pragma once
#include "vec2.hpp"

namespace VPanic {

	struct Space { // NOTE: this is stupid
		
		int x;
		int y;
		int w;
		int h;
			
		Space(int t_x = 0, int t_y = 0, int t_w = 0, int t_h = 0)
			: x(t_x), y(t_y), w(t_w), h(t_h) {}
		
		Space(const vec2& t_pos, int t_w, int t_h)
			: x(t_pos.x), y(t_pos.y), w(t_w), h(t_h) {}

		vec2 xy() const; // return position in vec2
		vec2 wh() const; // return size in vec2
		
		bool empty() const;

	};

}
