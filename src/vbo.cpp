#include <GL/glew.h>
#include "vbo.hpp"

namespace VPanic {
	
	VBO::~VBO() { 
		del();
   	}
	
	void VBO::data(const float* t_data) {
		bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(t_data) * sizeof(float), t_data, GL_STATIC_DRAW);
	}
	
	void VBO::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VBO::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VBO::gen() {
		glGenBuffers(1, &m_id);
	}

	void VBO::del() {
		unbind();
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}


}
