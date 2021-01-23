#include <GL/glew.h>
#include "ibo.hpp"

namespace VPanic {
	
	IBO::~IBO() { 
		del();
   	}
	
	void IBO::data(const uint32_t* t_indices) {
		bind();
		m_index_count = sizeof(t_indices) * sizeof(uint32_t);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count, t_indices, GL_STATIC_DRAW);
	}
	
	void IBO::bind() {
		// NOTE: this gets loaded into VAO, but make sure everything works first
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IBO::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IBO::gen() {
		glGenBuffers(1, &m_id);
	}

	void IBO::del() {
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	
	uint32_t IBO::index_count() const {
		return m_index_count;
	}
}
