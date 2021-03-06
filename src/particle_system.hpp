#pragma once
#include <vector>

#include "color.hpp"
#include "shape_array.hpp"

//
// this is going to change ...
// 

namespace vpanic {


	struct Particle {
	
		glm::vec3 pos              { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 vel              { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 acc              { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 scale            { glm::vec3(0.4f, 0.4f, 0.4f) };
		float rotation              { 0.0f };
		float lifetime              { 0.0f };
		float max_lifetime          { 2.0f };
		bool dead                  { true };
		Color color                { Color(255, 255, 255) };
		glm::mat4 _matrix          { glm::mat4(1.0f) };
	};


	class ParticleSystem {
	public:


		void init();
		void update(const Shader& t_shader, const float t_delta_time, const glm::mat4& t_camera_view);
		void particle_update_callback(void(*t_callback)(Particle*));

	private:
	
		uint32_t m_max_count { 10000 };
		std::vector<Particle> m_particles;
		ShapeArray m_shape_array;

		void(*m_update_callback)(Particle*) { nullptr };
	};

}
