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

		vec2 get_real_pos();
		vec2 get_pos();	
		bool button_down(int t_button);
	}
}


