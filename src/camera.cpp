#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "utils.hpp"

namespace vpanic {

	void Camera::update() {
		front = glm::normalize(glm::vec3(
					cos(glm::radians(yaw))*cos(glm::radians(pitch)),
					sin(glm::radians(pitch)),
					sin(glm::radians(yaw))*cos(glm::radians(pitch))));

		view = glm::translate(view, pos);
		view = glm::lookAt(pos, pos+front, glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(fov), aspect_ratio, z_near, z_far);
		
		if(roll != 0.0f) {
			projection = glm::rotate(projection, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}

	glm::vec3 Camera::_rot_xz(const float t_x, const float t_z) {
		return glm::vec3(cos(glm::radians(yaw))*cos(glm::radians(t_x)), 0, sin(glm::radians(yaw))*cos(glm::radians(t_z)));
	}

	void Camera::move(MoveDir t_direction, const float t_speed) {
		const float s = t_speed*delta_time;
		switch(t_direction) {	
			case MoveDir::UP:
				pos.y += s;
				break;
			
			case MoveDir::DOWN:
				pos.y -= s;
				break;
			
			case MoveDir::LEFT:
				if(freecam) {
					pos -= glm::cross(front, m_up)*s;
				} else {
					pos -= (glm::cross(_rot_xz(front.x, front.z), m_up)*s);
				}
				break;
			
			case MoveDir::RIGHT:
				if(freecam) {
					pos += glm::cross(front, m_up)*s;
				} else {
					pos += (glm::cross(_rot_xz(front.x, front.z), m_up)*s);
				}
				break;
			
			case MoveDir::FORWARD:
				if(freecam) {
					pos += front*s;
				} else {
					pos.x += cos(glm::radians(yaw))*cos(glm::radians(front.x))*s;
					pos.z += sin(glm::radians(yaw))*cos(glm::radians(front.z))*s;
				}
				break;
			
			case MoveDir::BACK:
				if(freecam) {
					pos -= front*s;
				} else {
					pos.x -= cos(glm::radians(yaw))*cos(glm::radians(front.x))*s;
					pos.z -= sin(glm::radians(yaw))*cos(glm::radians(front.z))*s;
				}
				break;

			default: break;
		}
	}
		
	void Camera::look_at_point(const glm::vec3& t_point) {
	}
	
	void Camera::look_at_mouse(const MouseData& t_data) {
		const float s = sensetivity;
		yaw += (t_data.delta_x)*s;
		pitch += (-t_data.delta_y)*s;
		clamp<float>(pitch, -89.9f, 89.9f);
	}
	
	glm::vec3 Camera::point_from_front(const float t_distance) {
		return glm::vec3(
					pos.x+cos(glm::radians(yaw))*(t_distance*cos(glm::radians(pitch))),
					pos.y+t_distance*sin(glm::radians(pitch)),
					pos.z+sin(glm::radians(yaw))*(t_distance*cos(glm::radians(pitch))));
	}



}
