#include "libs/gl3w.h"

#include "particle_system.hpp"
#include "utils.hpp"

namespace vpanic {

	
	void ParticleSystem::init() {
		std::vector<Vertex> data;
		add_plane_data(data);
		
		m_shape_array.load(data);
		m_shape_array.reserve(m_max_count);
		m_particles.resize(m_max_count);
	}


	void ParticleSystem::update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view) {		
		for(size_t i = 0; i < m_particles.size(); i++) {
			Particle* p = &m_particles[i];
			p->lifetime += t_delta_time;
			if(p->lifetime >= p->max_lifetime) {
				p->dead = true;
			}
			if(m_update_callback != nullptr) {
				m_update_callback(p);
			}

			//p->_matrix = glm::mat4(1.0f);
			//p->_matrix = glm::translate(p->_matrix, p->pos);
			//rotate_matrix_to_matrix(p->_matrix, t_camera_view);
			//p->_matrix = glm::scale(p->_matrix, p->scale);

			m_shape_array.set_matrix(i, m_particles[i]._matrix);
		}

		//glDisable(GL_DEPTH_TEST); ... oops!
		glDepthMask(GL_FALSE);
		m_shape_array.draw(t_shader);
		glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
	}
	
	void ParticleSystem::particle_update_callback(void(*t_callback)(Particle*)) {
		m_update_callback = t_callback;
	}


}


