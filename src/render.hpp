#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "shape.hpp"

#include "light.hpp"
#include "material.hpp"
#include "camera.hpp"

namespace VPanic {

	class Render {
	public:


		void add_light(const Light& t_light);
	//	void update_camera(Camera& t_cam);

		void draw(const Shape& t_shape, const Shader& t_shader, const Camera& t_cam);
		
		void wireframe(const bool b);

	private:

		Light m_light;
	
	};
}







