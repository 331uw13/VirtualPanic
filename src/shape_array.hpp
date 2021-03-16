#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "shader.hpp"
#include "vertex.hpp"

namespace vpanic {

	class ShapeArray {
	public:

		//Color color { Color(5, 240, 5) };

		//void place_matrices(const std::vector<Matrix>& t_matrices);
		void place_colors(void* t_colors, const size_t t_size);
		void place_matrices(void* t_matrices, const size_t t_size);
		
		void set_color(const uint32_t t_index, const Color& t_col);
		void set_matrix(const uint32_t t_index, const Matrix& t_mat);
		void reserve(const uint32_t t_size);
		uint32_t size() const;

		// -------------------

		void load(const std::vector<Vertex>& t_data, const int t_settings = 0);
		void unload();
		bool is_loaded() const;
		uint8_t get_type() const;
	
		void update_vertex(const Vertex& t_vertex, const int t_index);
		void update_vertices(const std::vector<Vertex>& t_vertices);

		void draw(const Shader& t_shader) const;	

		ShapeArray(const std::vector<Vertex>& t_data, const int t_settings = 0);
		ShapeArray() {}
		~ShapeArray();

	private:

		bool m_loaded       { false };
		uint32_t m_matrix_buffer  { 0 };
		uint32_t m_color_buffer { 0 };
		uint32_t m_vao      { 0 };
		uint32_t m_vbo      { 0 };
		uint32_t m_reserved { 0 };
		uint8_t  m_type     { 0 };
		uint32_t m_draw_data_size { 0 };

	};

}
