#include <SDL2/SDL.h>
#include "mouse.hpp"
#include "utils.hpp"

namespace vpanic {

	Vec2 mouse::get_pos() {
		int x = 0;
		int y = 0;
		SDL_GetMouseState(&x, &y);
		return Vec2(static_cast<float>(x), static_cast<float>(y));
	}

	bool mouse::button_down(const int t_button) {
		return SDL_GetMouseState(0, 0) & SDL_BUTTON(1+t_button);
	}

}
