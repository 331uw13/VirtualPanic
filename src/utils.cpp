#include <random>

#include "utils.hpp"
#include "messages.hpp"

namespace VPanic {

	static int g_seed  { 0 };

	float norm(float t_v, float t_min, float t_max) {
		return (t_v - t_min) / (t_max - t_min);
	}
	
	float map(int t_value, int t_src_min, int t_src_max, int t_dst_min, int t_dst_max) {
		return lerp(t_dst_min, t_dst_max, norm(t_value, t_src_min, t_src_max));
	}

	bool out_of_bounds(const Space& t_space, const vec2& t_size) {
		return ((t_space.x + t_space.w < 0) || (t_space.x > t_size.x)
				|| (t_space.y + t_space.h < 0) || (t_space.y > t_size.y));
	}
	
	bool out_of_bounds(const vec2& t_point, const vec2& t_size) {
		return out_of_bounds(Space(t_point, 1, 1), t_size);
	}

	float random(float t_min, float t_max) {
		return t_min + static_cast<float>(fast_rand()) / (static_cast<float>(0x7FFF / (t_max - t_min)));
	}

	int lerp(int t_a, int t_b, float t) {
		return t_a + (t_b - t_a) * t;
	}

	Color rgb_lerp(Color t_color1, Color t_color2, float t) {
		return Color(
				lerp(t_color1.r, t_color2.r, t),
				lerp(t_color1.g, t_color2.g, t),
				lerp(t_color1.b, t_color2.b, t),
				lerp(t_color1.a, t_color2.a, t));
	}
	
	Color rgb_invert(Color t_color) {
		return Color(
				255 - t_color.r,
				255 - t_color.g,
				255 - t_color.b
				);
	}
	
	Color copy_dim_color(Color t_color, int t_a) {
		return Color(
				t_color.r,
				t_color.g,
				t_color.b, 
				t_a
				);
	
	}

	void set_seed(const int t_seed) {
		g_seed = t_seed;
	}
	
	int fast_rand() {
		g_seed = (214013 * g_seed + 2531011);
  		return (g_seed >> 16) & 0x7FFF;
	}
	
	bool get_circle_points(const vec2& t_pos, const float t_radius, std::vector<vec2>* t_out) {
		bool found_any = false;
		for(int x = -t_radius; x < t_radius; x++) {
			const int h = sqrt(t_radius * t_radius - x * x);
			for(int y = -h; y < h; y++) {
				t_out->push_back(vec2(t_pos.x + x, t_pos.y + y));
				found_any = true;
			}
		}
		return found_any;
	}

	bool get_line_points(const vec2& t_p1, const vec2& t_p2, std::vector<vec2>* t_out) {
		bool found_any = false;
		vec2 pos = t_p1;
		const int dx = t_p2.x - t_p1.x;
		const int dy = t_p2.y - t_p1.y;
		const int w = (dx < 0) ? -1 : 1;
		const int h = (dy < 0) ? -1 : 1;

		const int adx = abs(dx);
		const int ady = abs(dy);
		const bool swap = (ady > adx);

		vec2 p1(w, h);
		vec2 p2((swap ? 0 : w), (swap ? h : 0));

		int l = (swap ? ady : adx);
		int s = (swap ? adx : ady);
		int n = l >> 1;

		for(int i = 0; i < l; i++) {
			n += s;
			if(n > l) {
				pos += p1;
				n -= l;
			} else {
				pos += p2;
			}
			t_out->push_back(pos);
			found_any = true;
		}

		return found_any;
	}


}
