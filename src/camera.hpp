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
		float aspect_ratio  { 1.0f };
		float sensetivity   { 0.145f };
		float move_speed    { 0.15f }; // NOTE: when creating entity class then move this
		float z_far   { 105.f };
		float z_near  { 0.15f };

		glm::mat4 view        { glm::mat4(1.0f) };
		glm::mat4 projection  { glm::mat4(1.0f) };
	
		void update(const Shader& t_shader); 
	};

}
