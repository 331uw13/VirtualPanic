#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
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

		Vec3 pos       { Vec3(0.0f) };
		Vec3 scale     { Vec3(1.0f) };
		Vec3 rotation  { Vec3(0.0f) };	
		Color color    { Color(255) };

		void enable_outline(Shader* t_shader_ptr);
		void disable_outline();
		bool is_outline_enabled() const;

		mutable float outline_thickness { 1.16f };
		float line_thickness { 1.1f };

		void load(const std::vector<Vertex>& t_data, const int t_settings = 0);
		void unload();
		bool is_loaded() const;
		uint8_t get_type() const;

		void set_model_matrix(const Matrix& t_matrix);
		//void set_model_matrix(const glm::mat4& t_matrix);
		void update_vertex(const Vertex& t_vertex, const int t_index);
		void update_vertices(const std::vector<Vertex>& t_vertices);
		
		void draw(const Shader& t_shader) const;		

		Shape(const std::vector<Vertex>& t_data, const Vec3& t_pos, const Color& t_color, const int t_settings = 0)
			: pos(t_pos), color(t_color) 
		{ load(t_data, t_settings); }
		
		Shape() : m_type(0) {}
		
	private:

		bool m_loaded  { false };
		uint32_t m_vao { 0 };
		uint32_t m_vbo { 0 };
		uint32_t m_draw_data_size { 0 };
		uint8_t m_type;
		
		bool m_outline { false };
		Shader* m_outline_shader { nullptr };

		mutable bool m_has_user_model_matrix { false };
		Matrix m_user_model_matrix { Matrix(1.0f) };
		//glm::mat4 m_user_model_matrix { glm::mat4(1.0f) };

	};
}




