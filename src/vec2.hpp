#pragma once

namespace vpanic {

	struct Vec2 {
		
		float x  { 0.0f };
		float y  { 0.0f };


		Vec2(const float t_xy)
			: x(t_xy), y(t_xy) {}

		Vec2(const float t_x, const float t_y)
			: x(t_x), y(t_y) {}

		Vec2() {}

	};

}


