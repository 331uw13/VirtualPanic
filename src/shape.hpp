#pragma once
#include <GL/glew.h>

#include "color.hpp"
#include "vec.hpp"

namespace VPanic {

	struct Shape {
		
		std::vector<float> vertices;

		vec3 pos;
		Color color;
		
		vec3 rotation  { vec3(0.0f, 0.0f, 0.0f) };
		vec3 scale     { vec3(0.0f, 0.0f, 0.0f) };

		uint32_t vao { 0 };


		void load(const std::vector<float>& t_data);
		void unload();
		bool is_loaded { false };

		Shape(const std::vector<float>& t_data, const vec3& t_pos, const Color& t_color = Color())
			: pos(t_pos), color(t_color) { load(t_data); }
		
		Shape() {}

	private:
		uint32_t m_vbo { 0 };
	};
}




