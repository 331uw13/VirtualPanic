#pragma once
#include <glm/vec3.hpp>

namespace VPanic {

	struct Vertex {

		glm::vec3 point;
		glm::vec3 normal;

		Vertex(const float t_x, const float t_y, const float t_z,
				const float t_nx, const float t_ny, const float t_nz)
			: point(glm::vec3(t_x, t_y, t_z)), normal(glm::vec3(t_nx, t_ny, t_nz)) {}
		
		Vertex(const float t_x, const float t_y, const float t_z)
			: point(glm::vec3(t_x, t_y, t_z)), normal(glm::vec3(0.0f, 0.0f, 0.0f)) {}

		Vertex(const glm::vec3& t_point)
			: point(glm::vec3(t_point.x, t_point.y, t_point.z)), normal(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	};

}
