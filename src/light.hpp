#pragma once
#include "color.hpp"
#include "vec.hpp"

namespace VPanic {

	struct Light {
		
		vec3 pos;
		Color color;

		Light(const vec3& t_pos, const Color& t_color)
			: pos(t_pos), color(t_color) {}

		Light() : pos(vec3(0.0f)), color(Color()) {}

	};



}
