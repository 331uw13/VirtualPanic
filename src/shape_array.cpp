#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shape_array.hpp"
#include "messages.hpp"

#define VEC3_SIZE  0xC

namespace VPanic {
	
	ShapeArray::ShapeArray(const std::vector<float>& t_data) {
		load(t_data);	
	}
	
	ShapeArray::~ShapeArray() {
		unload();
	}
	
	void ShapeArray::set(const glm::vec3& t_pos, uint32_t t_index) {
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*VEC3_SIZE, VEC3_SIZE, &t_pos);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void ShapeArray::place(const std::vector<glm::vec3>& t_positions) {
		if(!m_loaded) { load({ }); }
		const uint32_t size = t_positions.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glBufferData(GL_ARRAY_BUFFER, size*VEC3_SIZE, &t_positions[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_reserved = size;
	}
	
	void ShapeArray::reserve(const uint32_t t_size) {
		if(!m_loaded) { load({ }); }
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glBufferData(GL_ARRAY_BUFFER, t_size*VEC3_SIZE, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_reserved = t_size;
	}
	
	void ShapeArray::load(const std::vector<float>& t_data) {
		if(!m_loaded) { 
			glGenBuffers(1, &m_ibuffer);
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
	   	}

		glBindVertexArray(m_vao);

		if(!t_data.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, t_data.size() * sizeof(float), &t_data[0], GL_STATIC_DRAW);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VEC3_SIZE, (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);

		m_draw_data_size = t_data.size()/2;
		m_loaded = true;
	}
	
	void ShapeArray::unload() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibuffer);
		glDeleteVertexArrays(1, &m_vao);
		
		m_vbo = 0;
		m_vao = 0;
		m_ibuffer = 0;
		m_count = 0;
		m_draw_data_size = 0;
		m_loaded = false;
	}
	
	bool ShapeArray::is_loaded() const {
		return m_loaded;
	}
	
	uint32_t ShapeArray::size() const {
		int size = 0;
		glBindBuffer(GL_ARRAY_BUFFER, m_ibuffer);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		size /= VEC3_SIZE;
		return size;
	}
	
	void ShapeArray::draw(const Shader& t_shader) const {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		t_shader.set_mat4("model", model);

		glBindVertexArray(m_vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, m_draw_data_size, m_reserved);
	}

}





