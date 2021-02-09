#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shape.hpp"
#include "messages.hpp"

#define VEC3_SIZE  0xC

namespace VPanic {

	bool Shape::is_loaded() const {
		return m_loaded;
	}

	void Shape::load(const std::vector<Vertex>& t_data) {
		if(m_loaded) { unload(); }
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*t_data.size(), &t_data[0], GL_STATIC_DRAW);

		// points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VEC3_SIZE*2, 0);
		glEnableVertexAttribArray(0);
	
		// normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VEC3_SIZE*2, (void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		m_draw_data_size = t_data.size();
		m_loaded = true;
	}

	void Shape::unload() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		
		m_vbo = 0;
		m_vao = 0;
		m_loaded = false;
	}

	void Shape::draw(const Shader& t_shader) const {
		if(!m_loaded) { return; }	

		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, scale);
	
		// TODO: rotation

		t_shader.use();
		t_shader.set_vec3("shape.pos", pos);
		t_shader.set_color("shape.color", color);
		t_shader.set_mat4("model", model);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_draw_data_size);
	}

}






