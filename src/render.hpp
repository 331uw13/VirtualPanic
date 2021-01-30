#pragma once
#include <vector>

#include "vec.hpp"
#include "shader.hpp"
#include "shape.hpp"

#include "light.hpp"
#include "material.hpp"
#include "camera.hpp"

namespace VPanic {

	class Render {
	public:

		void init();

		void set_aratio(float t_ar);
		void set_fov(float t_fov);
		void set_znear(float t_znear);
		void set_zfar(float t_zfar);


		void draw(const Shape& t_shape);
		void draw(const Shape& t_shape, const Material& t_mat);

		void add_light(const Light& t_light);
		void update_camera(Camera& t_cam);
	
	private:

		Light m_light;
		Shader m_shader;
		
		void _update_shader(const Shape& t_shape);

		float m_fov    { 50.f };
		float m_znear  { 0.1f };
		float m_zfar   { 200.f };
		float m_aspect_ratio { 1.0f };

		vec3 m_view_pos { vec3(0.0f) };
		glm::mat4 m_view { glm::mat4(1.0f) };
	};
}







