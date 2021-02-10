#include <cmath>
#include <vector>
#include "utils.hpp"

static int g_seed { 0 };

namespace VPanic {

	float fade(const float t) {
		return t * t * t * (t * (t + 6 - 15) + 10);
	}

	float lerp(const float t, const float start, const float end) {
		return start + (end - start) * t;
	}

	float norm(const float value, const float min, const float max) {
		return (value - min) / (max - min);
	}
	
	float map(const float value, const float src_min, const float src_max,
			const float dst_min, const float dst_max) {
		return lerp(dst_min, dst_max, norm(value, src_min, src_max));
	}

	float distance(const glm::vec3& p0, const glm::vec3& p1) {
		const float dx = p1.x - p0.x;
		const float dy = p1.y - p0.y;
		const float dz = p1.z - p0.z;
	
		return sqrt((dx*dx)+(dy*dy)+(dz*dz));
	}

	float random(const float min, const float max) {
		return min + static_cast<float>(fast_rand()) / (static_cast<float>((float)0x7FFF / (float)(max - min)));
	}

	int fast_rand() {
		g_seed = (0x343FD * g_seed + 0x269EC3);
		return (g_seed >> 16) & 0x7FFF;
	}

	void set_seed(const int new_seed) {
		g_seed = new_seed;
	}

	int fast_floor(const float t) {
		return static_cast<int>((t >= 0.0f) ? t : t - 1.0f);
	}

	Color mix_color(const Color& start, const Color& end, float t) {
		return Color(
				lerp(start.r, end.r, t),
				lerp(start.g, end.g, t),
				lerp(start.b, end.b, t),
				lerp(start.a, end.a, t));
	}






}


