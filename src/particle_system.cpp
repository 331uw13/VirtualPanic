#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.hpp"
#include "utils.hpp"
#include "settings.hpp"
#include "console.hpp"


namespace vpanic {

	void ParticleSystem::init() {
		m_shapes.unload();
		std::vector<Vertex> data;
		//add_plane_data(data, DOUBLE_SIDE);
		add_box_data(data);
		set_normals(data);

		m_shapes.reserve(m_max_count);
		m_shapes.load(data);

		m_particles.clear();
		m_matrices.clear();

		m_particles.resize(m_max_count);
		m_matrices.resize(m_max_count);
	}
	
	void ParticleSystem::max_count(const uint32_t t_max_particles) {
		m_max_count = t_max_particles;
		init();
	}

	void ParticleSystem::add(const glm::vec3& t_pos, const glm::vec3& t_vel, const glm::vec3& t_acc, const float t_max_lifetime, const float t_scale) {
		const uint32_t index = _find_next_dead();
		m_particles[index] = 
			Particle {
			   	t_pos, // position
			   	t_vel, // velocity
			   	t_acc, // acceleration
				glm::vec3(t_scale),
				glm::vec3(0.0f),
				index, // id/index
			   	t_max_lifetime, // when particle should die
			   	Timer(), // used to know how long this particle is lived (TODO: do this with delta time)
				false
		};
	}
	
	uint32_t ParticleSystem::_find_next_dead() {
		for(uint32_t i = 0; i < m_particles.size(); i++) {
			if(m_particles[i].dead) {
				return i;
			}
		}
		return 0;
	}

	void ParticleSystem::clear() {
		m_particles.clear();
	}

	void ParticleSystem::draw(const Shader& t_shader, const Camera& t_cam) {

		uint32_t alive_count = 0;

		for(uint32_t i = 0; i < m_particles.size(); i++) {
			Particle& p = m_particles[i];

			if(p.timer.elapsed_ms() > p.max_lifetime) {
				p.dead = true;
				continue;
			}

			alive_count++;

			p.pos += p.velocity;
			p.velocity += p.acceleration;

			// clamp velocity for testing, remove this later.
			clamp<float>(p.velocity.x, -10.f, 10.f);
			clamp<float>(p.velocity.y, -10.f, 10.f);
			clamp<float>(p.velocity.z, -10.f, 10.f);
			// --------

			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), p.pos);
			matrix = glm::scale(matrix, p.scale);

			m_matrices[p.id] = matrix;

		}
		
		m_shapes.place(m_matrices);
		m_shapes.draw(t_shader);
	
		Console::instance().print("Particles alive: %li", alive_count);
	}



}
