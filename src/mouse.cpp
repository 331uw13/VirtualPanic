#include "mouse.hpp"

namespace VPanic {

	bool Mouse::button_down(int t_button) {
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1+(int)t_button);
	}
}
