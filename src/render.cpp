#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render.hpp"
#include "messages.hpp"

namespace VPanic {


	void Render::add_light(const Light& t_light) {
		m_light = t_light;
	}
	
	void Render::wireframe(const bool b) {
		glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
	}
	
	void Render::draw(const Shape& t_shape, const Shader& t_shader, const Camera& t_cam) {
		
		// NOTE: this should be saved in 'Shape'
		glm::mat4 model(1.0f); 
	
		// NOTE: this should be probably too handled in 'Shape::update()'
		model = glm::translate(model, glm::vec3(t_shape.pos.x, t_shape.pos.y, t_shape.pos.z));
		model = glm::scale(model, glm::vec3(t_shape.scale.x, t_shape.scale.y, t_shape.scale.z));
		model = glm::rotate(model, glm::radians(t_shape.rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(t_shape.rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(t_shape.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
		t_shader.use();
			
		// NOTE: this should be handled from user side, not in library
		t_shader.set_color("u_color", t_shape.color);
		t_shader.set_color("u_light_color", m_light.color);
		t_shader.set_vec3("u_light_pos", m_light.pos);
		t_shader.set_vec3("u_view_pos", t_cam.pos);
	
		// NOTE: this probably can be handled from library if added optional texture support and other things
		// 			and then vertex shader can be always loaded from memory
		// 			but this might be bad idea xd
		t_shader.set_mat4("proj", t_cam.projection);
		t_shader.set_mat4("view", t_cam.view);
		t_shader.set_mat4("model", model);
		
		glBindVertexArray(t_shape.vao);
		glDrawArrays(GL_TRIANGLES, 0, t_shape.vertices.size()/2);
	}

}










