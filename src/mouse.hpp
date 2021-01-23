#pragma once
#include "vec.hpp"

namespace VPanic {

	namespace Mouse {
		
		enum {
			LEFT,
			MIDDLE,
			RIGHT
		};
		
		Vec get_pos();
		bool button_down(int t_button);

	}
}
