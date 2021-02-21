#pragma once
#include <vector>

#include "shape_array.hpp"
#include "timer.hpp"
#include "camera.hpp"

// NOTES:
// - remove '_find_next_dead' doesnt how i want and think better solution for this
// - save color in vertex with 'ShapeArray'

namespace vpanic {

	struct Particle {

		glm::vec3 pos;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		glm::vec3 scale;
		glm::vec3 rotation;
	
		uint32_t id;
		float max_lifetime;
		Timer timer;
		
		bool dead { false };

	};


	class ParticleSystem {
	public:
		
		void init();

		void max_count(const uint32_t t_max_particles);
		void add(const glm::vec3& t_pos, const glm::vec3& t_vel, const glm::vec3& t_acc, const float t_max_lifetime, const float t_scale);
		void clear();

		void draw(const Shader& t_shader, const Camera& t_cam);

	private:
		
		uint32_t m_max_count { 5000 };

		ShapeArray m_shapes;
		std::vector<Particle> m_particles;
		std::vector<glm::mat4> m_matrices;
			
		uint32_t _find_next_dead();

	};

}
