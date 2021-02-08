#pragma once
#include <glm/vec2.hpp>

namespace VPanic {

	namespace Mouse {
		
		enum {
			LEFT,
			MIDDLE,
			RIGHT
		};
		
		glm::vec2 get_pos();
		bool button_down(const int t_button);

	}
}
