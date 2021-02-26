#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <functional>

#include "color.hpp"
#include "vertex.hpp"

struct ImVec4;


namespace vpanic {

	float fade   (const float t);
	float lerp   (const float t, const float start, const float end);
	float norm   (const float value, const float min, const float max);
	float map    (const float value, const float src_min, const float src_max,
		   			const float dst_min, const float dst_max);

	float angle_between(const glm::vec2& p0, const glm::vec2& p1);
	float distance(const glm::vec3& p0, const glm::vec3& p1);

	float random (const float min, const float max);
	glm::vec3 random_point(const glm::vec3& min, const glm::vec3& max);
	void set_seed(const int new_seed);
	int fast_rand();

	template<typename T> void clamp(T& value, const T& min, const T& max) {
		value = ((value > max) ? max : (value < min) ? min : value);
	}

	Color mix_color(const Color& start, const Color& end, float t);

	Color illuminate(const Color& color, const int8_t t); // NOTE: needs better name maybe
	void invert_color(Color& color);

	ImVec4 color_to_imvec4(const Color& color);
	Color imvec4_to_color(const ImVec4& v4);
	void read_imgui_theme(const char* filename);

	void add_triangle_data(std::vector<Vertex>& out);
	void add_plane_data(std::vector<Vertex>& out, const int t_settings = 0);
	void add_box_data(std::vector<Vertex>& out);
	void add_sphere_data(std::vector<Vertex>& out);
	
	void set_normals(std::vector<Vertex>& out, const int t_settings = 0);

	// TODO: Color fade_colors(blabla);
	// TODO: uint32_t color_to_hex(const Color& t_color);
	// TODO: Color hex_to_color(const uint32_t);

	Color hex_to_color(const uint32_t hex);
	bool is_hex_string(const std::string& t_str);		


}




