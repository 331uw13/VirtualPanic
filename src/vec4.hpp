#pragma once

namespace vpanic {

	struct Vec4 {
		
		float x  { 0.0f };
		float y  { 0.0f };
		float z  { 0.0f };
		float w  { 0.0f };


		Vec4(const float t_xyzw)
			: x(t_xyzw), y(t_xyzw), z(t_xyzw), w(t_xyzw) {}

		Vec4(const float t_x, const float t_y, const float t_z, const float t_w)
			: x(t_x), y(t_y), z(t_z), w(t_w) {}

		Vec4() {}

	};

}


