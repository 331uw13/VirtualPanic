#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "color.hpp"

namespace VPanic {

	float fade   (float t);
	float lerp   (float start, float end, float t);
	float norm   (float value, float min, float max);
	float map    (float value, float src_min, float src_max, float dst_min, float dst_max);

	float distance(const glm::vec3& p0, const glm::vec3& p1);

	float random (float min, float max);

	int fast_rand();
	void set_seed(int new_seed);

	template<typename T> void clamp(T& value, const T& min, const T& max) {
		value = ((value > max) ? max : (value < min) ? min : value);
	}

	Color color_lerp(const Color& start, const Color& end, float t);


}
