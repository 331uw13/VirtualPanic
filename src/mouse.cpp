#include <SDL2/SDL.h>
#include "mouse.hpp"

namespace VPanic {

	Vec Mouse::get_pos() {
		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);
		return Vec(static_cast<float>(x), static_cast<float>(y));
	}

	bool Mouse::button_down(int t_button) {
		return SDL_GetMouseState(0, 0) & SDL_BUTTON(1+t_button);
	}

}
