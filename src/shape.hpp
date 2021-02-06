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

	class Shape {
	public:

		glm::vec3 pos       { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 rotation  { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale     { glm::vec3(1.0f, 1.0f, 1.0f) };

		Color color         { Color(0, 0, 0) };


		void load(const std::vector<float>& t_data, const int t_settings = 0);
		void unload();
		bool is_loaded() const;

		void draw(const Shader& t_shader) const;

		Shape(const std::vector<float>& t_data,
			   	const glm::vec3& t_pos,
			   	const Color& t_color,
			   	const int t_settings = 0)
			: pos(t_pos), color(t_color) 
		{ load(t_data, t_settings); }
		
		Shape(const std::vector<float>& t_data,
				const glm::vec3& t_pos,
				const glm::vec3& t_scale,
			   	const Color& t_color,
			   	const int t_settings = 0)
			: pos(t_pos), scale(t_scale), color(t_color)
		{ load(t_data, t_settings); }

		Shape() {}
		

	private:
		bool m_loaded  { false };
		int m_settings { 0 };
		uint32_t m_vao { 0 };
		uint32_t m_vbo { 0 };
		uint32_t m_draw_data_size { 0 };
		
	};
}




