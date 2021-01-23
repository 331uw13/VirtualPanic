#include "utils.hpp"

namespace VPanic {

	static int g_seed { 0 };

	float lerp(float start, float end, float t) {
		return start + (end - start) * t;
	}

	float norm(float value, float min, float max) {
		return (value - min) / (max - min);
	}
	
	float map(float value, float src_min, float src_max, float dst_min, float dst_max) {
		return lerp(dst_min, dst_max, norm(value, src_min, src_max));
	}

	float random(float min, float max) {
		return min + static_cast<float>(fast_rand()) / (static_cast<float>(0x7FFF / (max - min)));
	}

	int fast_rand() {
		g_seed = (0x343FD * g_seed + 0x269EC3);
		return (g_seed >> 16) & 0x7FFF;
	}

	void set_seed(int new_seed) {
		g_seed = new_seed;
	}

	
	Color color_lerp(const Color& start, const Color& end, float t) {
		return Color(
				lerp(start.r, end.r, t),
				lerp(start.g, end.g, t),
				lerp(start.b, end.b, t),
				lerp(start.a, end.a, t));
	}

}



