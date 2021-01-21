#pragma once
#include <SDL2/SDL.h>
#include "vec2.hpp"

namespace VPanic {
	namespace Mouse {
		
		enum {
			LEFT,
			MIDDLE,
			RIGHT
		};
/*

	ivec2 Mouse::get_real_pos() {
		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);
		return ivec2(x, y);
	}

	ivec2 Mouse::get_pos() {
		return get_real_pos();
	}

*/
		template<typename T> get_real_pos() {
		}

		template<typename T> get_pos() {
		}

		bool button_down(int t_button);
	}
}


