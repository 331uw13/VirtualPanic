#include <GL/glew.h>
#include <vector>

#include "shape.hpp"

namespace VPanic {

	void Shape::load(const std::vector<float>& t_data) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, t_data.size() * sizeof(float), &t_data[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);
		
		is_loaded = true;
		vertices = t_data;
	}

	void Shape::unload() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &vao);
		m_vbo = 0;
		vao = 0;
		is_loaded = false;
	}

}
