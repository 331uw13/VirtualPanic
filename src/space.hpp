#pragma once
#include <SDL2/SDL.h>
#include "vec2.hpp"

namespace VPanic {

	struct Space : public SDL_Rect {
			
		Space(const vec2& t_pos, const vec2& t_size) {
			x = t_pos.x;
			y = t_pos.y;
			w = t_size.x;
			h = t_size.y;
		}

		Space(const vec2& t_pos, int t_w, int t_h) {
			x = t_pos.x;
			y = t_pos.y;
			w = t_w;
			h = t_h;
		}

		Space(int t_x, int t_y, int t_w, int t_h) {
			x = t_x;
			y = t_y;
			w = t_w;
			h = t_h;
		}

		Space() {
			x = y = w = h = 0;
		}

		vec2 xy() const; // return position in vec2
		vec2 wh() const; // return size in vec2
		
		bool empty() const;

	};

}
