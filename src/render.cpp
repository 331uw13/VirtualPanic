#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render.hpp"
#include "messages.hpp"

namespace VPanic {

	void Render::init() {
		m_shader.load_shaders("vertex.glsl", "fragment.glsl");
	}

	void Render::set_aratio(float t_ar) {
		m_aspect_ratio = t_ar;
	}

	void Render::set_fov(float t_fov) {
		m_fov = t_fov;
	}
	
	void Render::set_znear(float t_znear) {
		m_znear = t_znear;
	}
	
	void Render::set_zfar(float t_zfar) {
		m_zfar = t_zfar;
	}

	void Render::add_light(const Light& t_light) {
		m_light = t_light;
	}
	
	void Render::update_camera(Camera& t_cam) {
		m_view_pos = vec3(t_cam.pos.x, t_cam.pos.y, t_cam.pos.z);
		glm::vec3 p(m_view_pos.x, m_view_pos.y, m_view_pos.z);
		
		glm::vec3 dir(
				cos(glm::radians(t_cam.yaw)) * cos(glm::radians(t_cam.pitch)),
				sin(glm::radians(t_cam.pitch)),
				sin(glm::radians(t_cam.yaw)) * cos(glm::radians(t_cam.pitch)));

		t_cam.front = glm::normalize(dir);
		m_view = glm::translate(m_view, p);
		m_view = glm::lookAt(p, p+t_cam.front, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Render::_update_shader(const Shape& t_shape) {		
		glm::mat4 proj(1.0f);
		glm::mat4 model(1.0f);

		proj = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_znear, m_zfar);
		model = glm::translate(model, glm::vec3(t_shape.pos.x, t_shape.pos.y, t_shape.pos.z));	
		model = glm::rotate(model, glm::radians(t_shape.rotation.x), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(t_shape.rotation.y), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(t_shape.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	
		m_shader.use();
		
		m_shader.set_color("u_color", t_shape.color);
		m_shader.set_color("u_light_color", m_light.color);
		m_shader.set_vec3("u_light_pos", m_light.pos);
		m_shader.set_vec3("u_view_pos", m_view_pos);

		m_shader.set_mat4("proj", proj);
		m_shader.set_mat4("view", m_view);
		m_shader.set_mat4("model", model);
		
		//m_view = glm::mat4(1.0f);
	}
	
	void Render::draw(const Shape& t_shape) {
		_update_shader(t_shape);
		m_shader.set_bool("u_flat_material", true);

		glBindVertexArray(t_shape.vao);
		glDrawArrays(GL_TRIANGLES, 0, t_shape.vertices.size()/2);
	}
	
	void Render::draw(const Shape& t_shape, const Material& t_mat) {
		_update_shader(t_shape);
		m_shader.set_bool("u_flat_material", false);
	
		m_shader.set_vec3("u_material.ambient", t_mat.ambient);
		m_shader.set_vec3("u_material.diffuse", t_mat.diffuse);
		m_shader.set_vec3("u_material.specular", t_mat.specular);
		m_shader.set_bool("u_material.shine", t_mat.shine);


		glBindVertexArray(t_shape.vao);
		glDrawArrays(GL_TRIANGLES, 0, t_shape.vertices.size()/2);
	}

}










