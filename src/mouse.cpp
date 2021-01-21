#include "mouse.hpp"

namespace VPanic {

	ivec2 Mouse::get_real_pos() {
		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);
		return ivec2(x, y);
	}

	ivec2 Mouse::get_pos() {
		return get_real_pos();
	}

	bool Mouse::button_down(int t_button) {
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1+(int)t_button);
	}
}
