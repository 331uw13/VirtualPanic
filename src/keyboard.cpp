#include <SDL2/SDL.h>
#include "keyboard.hpp"

namespace VPanic {

	bool Keyboard::keydown(Mod t_mod) {
		return ((int)t_mod > 0) ? (SDL_GetModState() == (int)t_mod) : SDL_GetKeyboardState(0)[SDL_SCANCODE_SPACE];
	}
	
	bool Keyboard::keydown(const uint8_t t_key) {
		return (SDL_GetKeyboardState(0)[((t_key - 0x61) + 4)]);
	}

}
