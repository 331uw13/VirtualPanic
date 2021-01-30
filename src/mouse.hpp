#pragma once
#include "vec.hpp"

namespace VPanic {

	namespace Mouse {
		
		enum {
			LEFT,
			MIDDLE,
			RIGHT
		};
		
		vec2 get_pos();
		bool button_down(int t_button);

	}
}
