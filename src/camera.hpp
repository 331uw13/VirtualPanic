#pragma once
#include <glm/glm.hpp>

#include "shader.hpp"

namespace VPanic {

	struct Camera {
		
		glm::vec3 pos    { glm::vec3(0.0f) };
		glm::vec3 front  { glm::vec3(0.0f, 0.0f, -1.0f) };
		
		float fov    { 70.f };
		float yaw    { 0.0f };
		float pitch  { 0.0f };
		float roll   { 0.0f };
		float z_near  { 0.1f };
		float z_far   { 1000.f };
		float aspect_ratio  { 1.0f };
		float move_speed    { 0.15f }; // NOTE: when creating entity class then move this
		float sensetivity   { 0.145f };

		glm::mat4 view        { glm::mat4(1.0f) };
		glm::mat4 projection  { glm::mat4(1.0f) };

		void update(const Shader& t_shader); 
	
		void look_at_point(const glm::vec3& t_point);
		void look_at_cursor();
		glm::vec3 point_from_front(const float t_distance);
		

	};

}
