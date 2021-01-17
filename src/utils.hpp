#pragma once
#include <vector>
#include <SDL2/SDL.h>

#include "vec2.hpp"
#include "color.hpp"
#include "space.hpp"

namespace VPanic {	

	template <typename T> 
	void clamp(T& t, const T& t_min, const T& t_max) {
		t = (t < t_min) ? t_min : (t > t_max) ? t_max : t;
	}

	float norm(float t_v, float t_min, float t_max);
	float map(int t_value, int t_src_min, int t_src_max, int t_dst_min, int t_dst_max);
	int random(int t_min, int t_max);
	bool out_of_bounds(const Space& t_space, const vec2& t_size);
	bool out_of_bounds(const vec2& t_point, const vec2& t_size);

	int lerp(int t_a, int t_b, float t);
	Color rgb_lerp(Color t_color1, Color t_color2, float t);	
	Color rgb_invert(Color t_color);
	
	//Color rgb_mix(Color t_color1, Color& t_color2);
	Color copy_dim_color(Color t_color, int t_a);
	
	void set_seed(const int t_seed);
	int fast_rand();

	bool get_circle_points(const vec2& t_pos, const float t_radius, std::vector<vec2>* t_out);
	bool get_line_points(const vec2& t_p1, const vec2& t_p2, std::vector<vec2>* t_out);

}

