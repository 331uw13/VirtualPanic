#pragma once
#include <glm/vec2.hpp>

namespace VPanic {

	struct MouseData {
		const float x;
		const float y;
		const float delta_x;
		const float delta_y;
	};

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
