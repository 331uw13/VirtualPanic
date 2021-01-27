#include <GL/glew.h>
#include "render.hpp"
#include "messages.hpp"


namespace VPanic {

	void Render::init() {
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		
		m_vbo.gen();
		m_ibo.gen();
	
		m_vbo.bind();
		m_ibo.bind();

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
		glEnableVertexAttribArray(0);

		message(MType::OK, "Initialized renderer");
	}

	void Render::draw_rect(const Rect& t_rect) {
		
		/*
		m_vbo.data(points);
		m_ibo.data(indx);
		glDrawElements(GL_TRIANGLES, m_ibo.index_count(), GL_UNSIGNED_INT, nullptr);
		*/
	}
}














