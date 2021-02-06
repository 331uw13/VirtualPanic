#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

namespace VPanic {

	void Camera::update(const Shader& t_shader) {
		front = glm::normalize(glm::vec3(
					cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
					sin(glm::radians(pitch)),
					sin(glm::radians(yaw)) * cos(glm::radians(pitch))
					));

		view = glm::translate(view, pos);
		view = glm::lookAt(pos, pos+front, glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(fov), aspect_ratio, z_near, z_far);
		
		if(roll != 0.0f) {
			projection = glm::rotate(projection, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		t_shader.use();
		t_shader.set_vec3("camera.pos", pos);
		t_shader.set_mat4("proj", projection);
		t_shader.set_mat4("view", view);
	}
}
