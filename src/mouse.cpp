#include "mouse.hpp"

namespace VPanic {

	vec2 Mouse::get_real_pos() {
		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);
		return vec2(x, y);
	}

	vec2 Mouse::get_pos() {
		return get_real_pos();
	}

	bool Mouse::button_down(int t_button) {
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1+(int)t_button);
	}
}
