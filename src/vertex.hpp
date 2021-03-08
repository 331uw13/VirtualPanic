#pragma once
//#include <glm/vec3.hpp>

#include "vec3.hpp"
#include "vec2.hpp"

namespace vpanic {

	struct Vertex {

		// TODO: arbitrary attributes?
		
		Vec3 point;
		Vec3 normal;
		Vec2 tex_coords;
			
		Vertex(const float t_x, const float t_y, const float t_z,
				const float t_nx, const float t_ny, const float t_nz,
				const float t_tx, const float t_ty)
			: point(Vec3(t_x, t_y, t_z)), 
			normal(Vec3(t_nx, t_ny, t_nz)), 
			tex_coords(Vec2(t_tx, t_ty)) {}

		Vertex(const float t_x, const float t_y, const float t_z,
				const float t_nx, const float t_ny, const float t_nz)
			: point(Vec3(t_x, t_y, t_z)), 
			normal(Vec3(t_nx, t_ny, t_nz)),
			tex_coords(Vec2(0.0f, 0.0f)) {}
			
		Vertex(const float t_x, const float t_y, const float t_z)
			: point(Vec3(t_x, t_y, t_z)), 
			normal(Vec3(0.0f, 0.0f, 0.0f)),
			tex_coords(Vec2(0.0f, 0.0f)) {}

		Vertex() {}


		/*
		glm::vec3 point;
		glm::vec3 normal;
		glm::vec2 tex_coords;
			
		Vertex(const float t_x, const float t_y, const float t_z,
				const float t_nx, const float t_ny, const float t_nz,
				const float t_tx, const float t_ty)
			: point(glm::vec3(t_x, t_y, t_z)), 
			normal(glm::vec3(t_nx, t_ny, t_nz)), 
			tex_coords(glm::vec2(t_tx, t_ty)) {}

		Vertex(const float t_x, const float t_y, const float t_z,
				const float t_nx, const float t_ny, const float t_nz)
			: point(glm::vec3(t_x, t_y, t_z)), 
			normal(glm::vec3(t_nx, t_ny, t_nz)),
			tex_coords(glm::vec2(0.0f, 0.0f)) {}
			
		Vertex(const float t_x, const float t_y, const float t_z)
			: point(glm::vec3(t_x, t_y, t_z)), 
			normal(glm::vec3(0.0f, 0.0f, 0.0f)),
			tex_coords(glm::vec2(0.0f, 0.0f)) {}

		Vertex() {}
*/	
	};

}
