#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shape.hpp"
#include "messages.hpp"

namespace VPanic {

	bool Shape::is_loaded() const {
		return m_loaded;
	}

	void Shape::load(const std::vector<float>& t_data, const int t_settings) {
		if(m_loaded) { unload(); }
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, t_data.size() * sizeof(float), &t_data[0], GL_STATIC_DRAW);

		uint8_t attrib_indx = 0;
		uint8_t draw_data_div = 1;
	
		glVertexAttribPointer(attrib_indx, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
		glEnableVertexAttribArray(attrib_indx);
		attrib_indx++;
		
		if(!(t_settings & NO_COLORED_LIGHT)) {	
			// normals
			glVertexAttribPointer(attrib_indx, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
			glEnableVertexAttribArray(attrib_indx);
			draw_data_div++;
			attrib_indx++;
		}

		if(t_settings & INCLUDED_COLOR_DATA) {
			// ...
		}

		// you dont want to actually include other data to triangle points
		m_draw_data_size = t_data.size() / draw_data_div;

		m_settings = t_settings;
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
		
		t_shader.use();
		t_shader.set_vec3("shape.pos", pos);
		t_shader.set_color("shape.color", color);
		t_shader.set_mat4("model", model);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, m_draw_data_size);
	}

}






