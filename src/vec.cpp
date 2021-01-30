#include "vec.hpp"

namespace VPanic {

	vec2 vec2::operator / (const vec2& t) {
		return vec2(x / (t.x == 0) ? 1 : t.x, y / (t.y == 0) ? 1 : t.y);
	}

	vec2 vec2::operator / (const float t) {
		const float d = (t == 0) ? 1 : t;
		return vec2(x / d, y / d);
	}

	vec2 vec2::operator - (const vec2& t) {
		return vec2(x - t.x, y - t.y);
	}
}
