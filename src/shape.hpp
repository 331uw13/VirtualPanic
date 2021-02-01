#pragma once
#include <glm/vec3.hpp>
//#include <glm/mat4.hpp>
#include <GL/glew.h>

#include "color.hpp"

namespace VPanic {

	struct Shape {
		
		std::vector<float> vertices;

		glm::vec3 pos       { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 rotation  { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale     { glm::vec3(1.0f, 1.0f, 1.0f) };

		Color color         { Color(0, 0, 0) };

		uint32_t vao { 0 };

		void load(const std::vector<float>& t_data);
		void unload();
		bool is_loaded { false };

		Shape(const std::vector<float>& t_data, const glm::vec3& t_pos, const Color& t_color = Color())
			: pos(t_pos), color(t_color) 
		{ load(t_data); }

		Shape() {}

	private:
		uint32_t m_vbo { 0 };
	};
}




