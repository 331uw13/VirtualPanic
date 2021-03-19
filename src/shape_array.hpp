#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "shader.hpp"
#include "vertex.hpp"

namespace vpanic {

	// NOTE: this needs better name
	class ShapeArray {
	public:

		void fill_colors(Color* t_colors, const size_t t_size);
		void fill_matrices(Matrix* t_matrices, const size_t t_size);
		void fill_point_pos(Vec3* t_positions, const size_t t_size);

		void set_color(const uint32_t t_index, const Color& t_col);
		void set_matrix(const uint32_t t_index, const Matrix& t_mat);
		void set_point_pos(const uint32_t t_index, const Vec3& t_pos);
		
		void reserve(const uint32_t t_size);
		uint32_t size() const;

		void load(const std::vector<Vertex>& t_data);
		void load_point();
		void unload();
		bool is_loaded() const;
		
		int get_type() const;
		void set_type(const int t_type);

		void update_vertex(const Vertex& t_vertex, const int t_index);
		void update_vertices(const std::vector<Vertex>& t_vertices);

		void draw(const Shader& t_shader) const;	

		ShapeArray() {}
		~ShapeArray();

	private:

		enum class Mode {
			SHAPE,
			POINT,
			NONE
		};

		void _init_attrib_pointers();

		Mode m_mode { Mode::NONE };
		int m_type { -1 };
		bool m_loaded { false };
		uint32_t m_draw_data_size { 0 };
	
		uint32_t m_position_buffer { 0 };
		uint32_t m_matrix_buffer   { 0 };
		uint32_t m_color_buffer    { 0 };
		uint32_t m_vao      { 0 };
		uint32_t m_vbo      { 0 };
		uint32_t m_reserved { 0 };

	};

}
