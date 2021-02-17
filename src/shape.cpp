#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "shape.hpp"
#include "messages.hpp"

#define VEC3_SIZE  0xC

namespace vpanic {
	
	bool Shape::is_loaded() const {
		return m_loaded;
	}

	void Shape::load(const std::vector<Vertex>& t_data, const int t_settings) {
		if(t_data.empty()) { return; }

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*t_data.size(), &t_data[0], GL_STATIC_DRAW);

		message(MType::DEBUG, "Shape::load(): %i bytes, %i vertices", sizeof(Vertex)*t_data.size(), t_data.size());

		const uint32_t stride = sizeof(glm::vec3)*2+sizeof(glm::vec2);

		// points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);
	
		// normals
		if(!(t_settings & NO_COLORED_LIGHT)) {
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, normal)));
			glEnableVertexAttribArray(1);
		}

		// texture coordinates
		//if(!(t_settings & NO_TEXTURE)) {
		//}
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex, tex_coords)));
		glEnableVertexAttribArray(2);	
		
		const size_t data_size = t_data.size();

		if(data_size >= 3) {
			m_type = GL_TRIANGLES;
		}
		else if(data_size == 2) {
			m_type = GL_LINES;
		}
		else {
			m_type = 0;
		}

		m_draw_data_size = data_size;
		m_loaded = true;

	}
	
	void Shape::unload() {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		
		m_vbo = 0;
		m_vao = 0;
		m_loaded = false;
	}
		
	void Shape::update_vertex(const Vertex& t_vertex, const int t_index) {
		if(!m_loaded) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, t_index*sizeof(Vertex), sizeof(Vertex), &t_vertex);
	}

	void Shape::update_vertices(const std::vector<Vertex>& t_vertices) {
		if(!m_loaded) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, t_vertices.size()*sizeof(Vertex), &t_vertices[0]);	
	}
		
	void Shape::draw(const Shader& t_shader) const {
		if(!m_loaded) { return; }	
		if(m_type == 0) { return; }

		glm::mat4 model(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);

		t_shader.use();
		t_shader.set_vec3("shape.pos", pos);
		t_shader.set_color("shape.color", color);
		t_shader.set_mat4("model", model);
		t_shader.set_int("use_offset", 0);
		
		if(m_type == GL_LINES) {
			glLineWidth(m_line_thickness);
		}

		glBindVertexArray(m_vao);
		glDrawArrays(m_type, 0, m_draw_data_size);
	}
	
	void Shape::line_thickness(const float t_value) {
		m_line_thickness = t_value;
	}
		
	uint8_t Shape::get_type() const {
		return m_type;
	}

}






