#include "libs/gl3w.h"

#include "particle_system.hpp"
#include "utils.hpp"


namespace vpanic {

	void ParticleSystem::init() {
		std::vector<Vertex> data;
		add_plane_data(data);
		set_normals(data);

		m_shape_array.load(data);
		m_shape_array.reserve(m_max_count);
		m_particles.resize(m_max_count);
		
	}

	void ParticleSystem::update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view) {		
		for(size_t i = 0; i < m_particles.size(); i++) {
			Particle* p = &m_particles[i];
			p->lifetime += (t_delta_time*1000.f);
			if(p->lifetime >= p->max_lifetime) {
				p->dead = true;
			}
			if(m_update_callback != nullptr) {
				m_update_callback(p);
			}

			p->_matrix.clear();
			p->_matrix.translate(p->pos);
			p->_matrix.copy_rotation(t_camera_view);
			p->_matrix.scale(p->scale);

			// TODO: test performance between setting matrices like this or first updating private array of matrices and setting that?
			m_shape_array.set_matrix(i, p->_matrix);
			m_shape_array.set_color(i, p->color);
		}

		glDepthMask(GL_FALSE);
		m_shape_array.draw(t_shader);
		glDepthMask(GL_TRUE);
	}
	
	void ParticleSystem::update_callback(void(*t_callback)(Particle*)) {
		m_update_callback = t_callback;
	}


}


