#pragma once
#include "vec2.hpp"

namespace vpanic {

	struct MouseData {
		const float x;
		const float y;
		const float delta_x;
		const float delta_y;
	};

	namespace mouse {
		
		enum {
			LEFT,
			MIDDLE,
			RIGHT
		};
		
		Vec2 get_pos();
		bool button_down(const int t_button);

	}
}
