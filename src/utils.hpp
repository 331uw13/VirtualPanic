#pragma once
#include <vector>
#include <string>
#include <functional>

#include "vec3.hpp"
#include "vec2.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "vertex.hpp"

struct ImVec4;
struct ImVec2;


namespace vpanic {

	float fade   (const float t);
	float lerp   (const float start, const float end, const float t);
	float norm   (const float value, const float min, const float max);
	float map    (const float value, const float src_min, const float src_max,
		   			const float dst_min, const float dst_max);

	float distance(const Vec3& p0, const Vec3& p1);

	float random (const float min, const float max);
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

	void set_plane_data(std::vector<Vertex>& out, const int t_settings = 0);
	void set_box_data(std::vector<Vertex>& out);
	void set_sphere_data(std::vector<Vertex>& out);
	
	void set_texcoords(std::vector<Vertex>& out);
	void set_normals(std::vector<Vertex>& out);

	void foreach_triangle(std::vector<Vertex>& vertices, const std::function<void(Vertex& p0, Vertex& p1, Vertex& p2)> f);

	// TODO: Color fade_colors(blabla);
	// TODO: uint32_t color_to_hex(const Color& t_color);
	
	Color hex_to_color(const uint32_t hex);
	bool is_hex_string(const std::string& t_str);		

	namespace ImGuiExt {
		bool ColorPicker(const char* label, ImVec4& color, const int flags = 0);
		bool ColorEdit(const char* label, ImVec4& color, const int flags = 0);
		bool TextEdit(const char* label, std::string* str, const ImVec2& size, const int flags = 0);
		void TextRGB(const char* fmt, ...);
	}

}




