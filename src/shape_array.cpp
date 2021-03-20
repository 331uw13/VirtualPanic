#include <cstring>
#include "libs/gl3w.h"

#include "shape_array.hpp"
#include "messages.hpp"
#include "internal.hpp"


namespace vpanic {
	
	ShapeArray::~ShapeArray() {
		unload();
	}
	
	void ShapeArray::fill_colors(Color* t_colors, const size_t t_size) {
		if(t_colors == nullptr) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(ptr == nullptr) { return; }
		
		memmove(ptr, t_colors, t_size*sizeof(Color));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void ShapeArray::fill_matrices(Matrix* t_matrices, const size_t t_size) {
		if(t_matrices == nullptr) { return; }
		if(m_mode != Mode::SHAPE) { return; }

		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(ptr == nullptr) { return; }
		
		memmove(ptr, t_matrices, t_size*sizeof(Matrix));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	void ShapeArray::fill_point_pos(Vec3* t_positions, const size_t t_size) {
		if(t_positions == nullptr) { return; }
		if(m_mode != Mode::POINT) { return; }
		
		glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if(ptr == nullptr) { return; }
		
		memmove(ptr, t_positions, t_size*sizeof(Vec3));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	void ShapeArray::set_matrix(const uint32_t t_index, const Matrix& t_mat) {
		if(m_mode != Mode::SHAPE) { return; }
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Matrix), sizeof(Matrix), &t_mat);		
	}
	
	void ShapeArray::set_color(const uint32_t t_index, const Color& t_col) {
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Color), sizeof(Color), &t_col);		
	}
	
	void ShapeArray::set_point_pos(const uint32_t t_index, const Vec3& t_pos) {
		if(m_mode != Mode::POINT) { return; }
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Vec3), sizeof(Vec3), &t_pos);		
	}

	void ShapeArray::reserve(const uint32_t t_size) {
		if(!m_loaded) { load({ }); }

		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, t_size*sizeof(Color), NULL, GL_STREAM_DRAW);

		if(m_mode == Mode::POINT) {
			glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer);
			glBufferData(GL_ARRAY_BUFFER, t_size*sizeof(Vec3), NULL, GL_STREAM_DRAW);
		}
		else if(m_mode == Mode::SHAPE){
			glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
			glBufferData(GL_ARRAY_BUFFER, t_size*sizeof(Matrix), NULL, GL_STREAM_DRAW);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_reserved = t_size;
	}

	void ShapeArray::load(const std::vector<Vertex>& t_data) {
		if(!m_loaded) {
			glGenBuffers(1, &m_matrix_buffer);
			glGenBuffers(1, &m_color_buffer);
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
	   	}
		glBindVertexArray(m_vao);
		m_mode = Mode::SHAPE;

		const size_t data_size = t_data.size();
		if(data_size > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, data_size*sizeof(Vertex), &t_data[0], GL_STATIC_DRAW);
			
			message(MType::DEBUG, "ShapeArray::load(): %i bytes, %i vertices", sizeof(Vertex)*t_data.size(), t_data.size());

			// TODO: option for GL_LINE_STRIP and GL_TRIANGLE_STRIP
			if(data_size >= 3) {
				m_type = GL_TRIANGLES;
			}
			else if(data_size == 2) {
				m_type = GL_LINES;
			}
			else {
				m_type = -1;
			}
		}
		
		_init_attrib_pointers();

		m_draw_data_size = t_data.size();
		m_loaded = true;
	}
	
	void ShapeArray::load_point() {
		if(!m_loaded) {
			glGenBuffers(1, &m_position_buffer);
			glGenBuffers(1, &m_color_buffer);
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
	   	}
		glBindVertexArray(m_vao);
		m_mode = Mode::POINT;

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), NULL, GL_STATIC_DRAW);

		_init_attrib_pointers();

		m_type = GL_POINTS;
		m_draw_data_size = 1;
		m_loaded = true;
	}
	
	void ShapeArray::_init_attrib_pointers() {

		const uint32_t stride = sizeof(Vec3)*2+sizeof(Vec2);

		// points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		// normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		// texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, tex_coords)));
		glEnableVertexAttribArray(2);	
	
		// color
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		
		if(m_mode == Mode::POINT) {
			// point position
			glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribDivisor(4, 1);
		}
		else if(m_mode == Mode::SHAPE) {
			// matrices
			glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
			for(int i = 0; i < 4; i++) {
				const int id = i+5;
				glVertexAttribPointer(id, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)(sizeof(Vec4)*i));
				glEnableVertexAttribArray(id);
				glVertexAttribDivisor(id, 1);
			}
		}
	}
		
	void ShapeArray::update_vertex(const Vertex& t_vertex, const int t_index) {
		if(!m_loaded) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Vertex), sizeof(Vertex), &t_vertex);
	}

	void ShapeArray::update_vertices(const std::vector<Vertex>& t_vertices) {
		if(!m_loaded) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, t_vertices.size()*sizeof(Vertex), &t_vertices[0]);	
	}

	void ShapeArray::unload() {
		if(!m_loaded) { return; }
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_color_buffer);
		glDeleteBuffers(1, &m_matrix_buffer);
		glDeleteVertexArrays(1, &m_vao);
		
		m_vbo = 0;
		m_vao = 0;
		m_matrix_buffer = 0;
		m_color_buffer = 0;
		m_draw_data_size = 0;
		m_type = -1;
		m_loaded = false;
	}
	
	bool ShapeArray::is_loaded() const {
		return m_loaded;
	}
	
	uint32_t ShapeArray::size() const {
		int size = 0;
		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		size /= sizeof(Matrix);
		return size;
	}
	
	void ShapeArray::draw(const Shader& t_shader) const {
		if(!m_loaded) { return; }
		if(m_type < 0) { return; }
		if(m_mode == Mode::NONE) { return; }

		//static Matrix model(1.0f);

		t_shader.use();
		//t_shader.set_mat4("model", model);
		t_shader.set_int("mode", internal::shader_mode__shape_array+(int)m_mode);

		glBindVertexArray(m_vao);
		glDrawArraysInstanced(m_type, 0, m_draw_data_size, m_reserved);
	}
	
	void ShapeArray::set_type(const int t_type) {
		m_type = t_type;
	}
	
	int ShapeArray::get_type() const {
		return m_type;
	}

}





