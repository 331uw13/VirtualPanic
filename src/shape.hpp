#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "color.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"

namespace vpanic {

	/*
	
		draw individual shape with its own position, rotation, scale and color.
		drawing too many of these will ofcourse slow your program down,
	   	check out 'ShapeArray' for drawing many shapes

	*/

	class Shape {
	public:

		glm::vec3 pos       { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale     { glm::vec3(1.0f, 1.0f, 1.0f) };
		glm::vec3 rotation  { glm::vec3(0.0f, 0.0f, 0.0f) };

		Color color         { Color(5, 240, 5) };

		void load(const std::vector<Vertex>& t_data, const int t_settings = 0);
		void unload();
		bool is_loaded() const;
		uint8_t get_type() const;

		void update_vertex(const Vertex& t_vertex, const int t_index);
		void update_vertices(const std::vector<Vertex>& t_vertices);
		
		void draw(const Shader& t_shader) const;		
		void line_thickness(const float t_value);

		Shape(const std::vector<Vertex>& t_data, const glm::vec3& t_pos,
			   	const Color& t_color, const int t_settings = 0)
			: pos(t_pos), color(t_color) 
		{ load(t_data, t_settings); }
		
		Shape() : m_type(0) {}
		
	private:

		bool m_loaded  { false };
		uint32_t m_vao { 0 };
		uint32_t m_vbo { 0 };
		uint32_t m_draw_data_size { 0 };

		float m_line_thickness { 1.0f };
		uint8_t m_type;

	};
}




