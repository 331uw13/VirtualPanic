#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "shape.hpp"
#include "messages.hpp"
#include "utils.hpp"


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
		
		
	void Shape::set_model_matrix(const Matrix& t_matrix) {
		m_has_user_model_matrix = true;
		m_user_model_matrix = t_matrix;
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
	
	void Shape::enable_outline(Shader* t_shader_ptr) {
		m_outline = true;
		m_outline_shader = t_shader_ptr;
	}

	void Shape::disable_outline() {
		m_outline = false;
		if(m_outline_shader != nullptr) {
			m_outline_shader = nullptr;
		}
	}

	bool Shape::is_outline_enabled() const {
		return m_outline;
	}
	
	void Shape::draw(const Shader& t_shader) const {
		if(!m_loaded) { return; }	
		if(m_type == 0) { return; }
		
		t_shader.use();

		Matrix model(1.0f);
		//glm::mat4 model(1.0f);

		if(!m_has_user_model_matrix) {
			model.translate(pos);
			//model = glm::translate(model, pos);
			
			if(!rotation.all(0.0f)) {
				model.rotate(Vec3(1.0f, 0.0f, 0.0f), rotation.x);
				model.rotate(Vec3(0.0f, 1.0f, 0.0f), rotation.y);
				model.rotate(Vec3(0.0f, 0.0f, 1.0f), rotation.z);
			}

			/*
			if(rotation != glm::vec3(0.0f)) {
				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			*/
			
			if(!scale.all(0.0f)) {
				model.scale(scale);
				//model = glm::scale(model, scale);
			}
		}
		else {
			model = m_user_model_matrix;
			m_has_user_model_matrix = false;
			//t_shader.set_mat4("model", m_user_model_matrix);
		}

		t_shader.set_vec3("shape.pos", pos);
		t_shader.set_mat4("model", model);
		t_shader.set_color("shape.color", color);
		t_shader.set_int("use_offset", 0);
		
		if(m_type == GL_LINES) {
			glLineWidth(line_thickness);
		}
/*
		const bool use_outline = (m_outline && m_outline_shader != nullptr);

		if(use_outline) {
			glStencilFunc(GL_ALWAYS, 1, 255);
			glStencilMask(255);
		}
*/		
		glBindVertexArray(m_vao);
		glDrawArrays(m_type, 0, m_draw_data_size);
/*
		if(use_outline) {
			
			clamp<float>(outline_thickness, 1.02f, 2.0f);
			
			m_outline_shader->use();
			m_outline_shader->set_vec3("shape.pos", pos);
			m_outline_shader->set_color("shape.color", color);
			m_outline_shader->set_mat4("model", glm::scale(model, glm::vec3(outline_thickness)));
			m_outline_shader->set_int("use_offset", 0);

			glDepthFunc(GL_ALWAYS);
			glStencilFunc(GL_NOTEQUAL, 1, 255);
			glStencilMask(0);
		
			glBindVertexArray(m_vao);
			glDrawArrays(m_type, 0, m_draw_data_size);

			glDepthFunc(GL_LESS);
			glStencilFunc(GL_ALWAYS, 1, 255);
			glStencilMask(255);
		}
		*/
	}
	
	uint8_t Shape::get_type() const {
		return m_type;
	}

}






