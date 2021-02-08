#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "color.hpp"
#include "shader.hpp"

namespace VPanic {

	/*
	
		draw individual shape with its own position, rotation, scale and color.
		drawing too many of these will ofcourse slow your program down,
	   	check out 'ShapeArray' for drawing many shapes

	*/

	struct Vertex {
		glm::vec3 point;
		glm::vec3 normal;

		Vertex(float x, float y, float z,
				float nx, float ny, float nz) {
			point = glm::vec3(x, y, z);
			normal = glm::vec3(nx, ny, nz);
		}

		Vertex() {}
	};

	class Shape {
	public:

		glm::vec3 pos       { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale     { glm::vec3(1.0f, 1.0f, 1.0f) };
		glm::vec3 rotation  { glm::vec3(0.0f, 0.0f, 0.0f) };

		Color color         { Color(5, 240, 5) };


		void load(const std::vector<Vertex>& t_data);
		void unload();
		bool is_loaded() const;

		void draw(const Shader& t_shader) const;

		Shape(const std::vector<Vertex>& t_data,
			   	const glm::vec3& t_pos,
			   	const Color& t_color)
			: pos(t_pos), color(t_color) 
		{ load(t_data); }
		
		Shape(const std::vector<Vertex>& t_data,
				const glm::vec3& t_pos,
				const glm::vec3& t_scale,
			   	const Color& t_color)
			: pos(t_pos), scale(t_scale), color(t_color)
		{ load(t_data); }

		Shape() {}
		

	private:
		
		bool m_loaded  { false };
		uint32_t m_vao { 0 };
		uint32_t m_vbo { 0 };
		uint32_t m_draw_data_size { 0 };
		
	};
}




