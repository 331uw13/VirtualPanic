#include <cstring>
#include "libs/gl3w.h"

#include "shape_array.hpp"
#include "messages.hpp"

//#define MATRIX_SIZE  0x40
//#define VEC3_SIZE  0xC


namespace vpanic {
	
	ShapeArray::ShapeArray(const std::vector<Vertex>& t_data, const int t_settings) {
		load(t_data, t_settings);
	}
	
	ShapeArray::~ShapeArray() {
		unload();
	}
	/*
	void ShapeArray::place(const std::vector<glm::mat4>& t_matrices) {
		if(!m_loaded) { load({ }); }
		const size_t size = t_matrices.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glBufferData(GL_ARRAY_BUFFER, size*MATRIX_SIZE, &t_matrices[0], GL_DYNAMIC_DRAW);
		m_reserved = size;
	}
	*/
	/*
	void ShapeArray::set(const uint32_t t_index, const glm::mat4& t_matrix) {
		//if(t_index >= m_reserved) { return; }
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*MATRIX_SIZE, MATRIX_SIZE, &t_matrix);		
	}
	*/
	
	void ShapeArray::place_colors(void* t_colors, const size_t t_size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memmove(ptr, t_colors, t_size*sizeof(Vec4));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void ShapeArray::place_matrices(void* t_matrices, const size_t t_size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memmove(ptr, t_matrices, t_size*sizeof(Matrix));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
	void ShapeArray::set_matrix(const uint32_t t_index, const Matrix& t_mat) {
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Matrix), sizeof(Matrix), &t_mat);		
	}
	
	void ShapeArray::set_color(const uint32_t t_index, const Color& t_col) {
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		const Vec4 color(t_col.r/255.f, t_col.g/255.f, t_col.b/255.f, t_col.a/255.f);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Vec4), sizeof(Vec4), &color);		
	}

	void ShapeArray::reserve(const uint32_t t_size) {
		if(!m_loaded) { load({ }); }

		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, t_size*sizeof(Vec4), NULL, GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		glBufferData(GL_ARRAY_BUFFER, t_size*sizeof(Matrix), NULL, GL_STREAM_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_reserved = t_size;
	}
	
	// ------------------------------

	void ShapeArray::load(const std::vector<Vertex>& t_data, const int t_settings) {
		if(!m_loaded) { 
			glGenBuffers(1, &m_matrix_buffer);
			glGenBuffers(1, &m_color_buffer);
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
	   	}

		glBindVertexArray(m_vao);

		if(!t_data.empty()) {
			const size_t data_size = t_data.size();
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, data_size*sizeof(Vertex), &t_data[0], GL_STREAM_DRAW);
		
			message(MType::DEBUG, "ShapeArray::load(): %i bytes, %i vertices", sizeof(Vertex)*t_data.size(), t_data.size());

			if(data_size >= 3) {
				m_type = GL_TRIANGLES;
			}
			else if(data_size == 2) {
				m_type = GL_LINES;
			}
			else {
				m_type = 0;
			}
		}
	
		const uint32_t stride = sizeof(Vec3)*2+sizeof(Vec2);

		// points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);

		// normals
		if(!(t_settings & NO_COLORED_LIGHT)) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, normal)));
			glEnableVertexAttribArray(1);
		}

		// texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, tex_coords)));
		glEnableVertexAttribArray(2);	
	
		// color
		glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		
		// matrices
		glBindBuffer(GL_ARRAY_BUFFER, m_matrix_buffer);
		for(int i = 0; i < 4; i++) {
			const int id = i+4;
			glVertexAttribPointer(id, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (void*)(sizeof(Vec4)*i));
			glEnableVertexAttribArray(id);
			glVertexAttribDivisor(id, 1);
		}
		
		m_draw_data_size = t_data.size();
		m_loaded = true;
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
		if(m_type == 0) { return; }
		
		static Matrix model(1.0f);

		t_shader.use();
		t_shader.set_mat4("model", model);
		t_shader.set_int("use_offset", 1);

		glBindVertexArray(m_vao);
		glDrawArraysInstanced(m_type, 0, m_draw_data_size, m_reserved);
	}
	
	uint8_t ShapeArray::get_type() const {
		return m_type;
	}

}





