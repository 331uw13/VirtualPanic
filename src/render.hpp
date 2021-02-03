#pragma once
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "shape.hpp"
#include "camera.hpp"

namespace VPanic {

	class Render {
	public:

		void draw(const Shape& t_shape, const Shader& t_shader, const Camera& t_cam);
		void wireframe(const bool b);

	private:
	
	};
}







