#pragma once
#include <glm/glm.hpp>

namespace VPanic {

	struct Camera {
		
		glm::vec3 pos    { glm::vec3(0.0f) };
		glm::vec3 front  { glm::vec3(0.0f, 0.0f, -1.0f) };	

		float pitch  { 0.0f };
		float yaw    { 0.0f };
		float roll   { 0.0f };

		float sensetivity { 0.01f };
		float move_speed  { 0.2f };

	};

}
