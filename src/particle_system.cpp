#include <cmath>
#include <thread>
#include "libs/gl3w.h"

#include "particle_system.hpp"
#include "utils.hpp"
#include "internal.hpp"
#include "messages.hpp"


namespace vpanic {

	void ParticleSystem::init() {
		m_blob_max_count = m_max_count/2;
		clamp<uint32_t>(m_blob_max_count, 1, m_blob_max_count);

		const int blob_count = ceil(m_max_count/m_blob_max_count);
		m_blobs.resize(blob_count);

		for(size_t i = 0; i < m_blobs.size(); i++) {
			Blob& b = m_blobs[i];
			b.shape_array.load_point();
			b.shape_array.reserve(m_blob_max_count);
			b.particles.resize(m_blob_max_count);
		}
	}

	void ParticleSystem::resize(const uint32_t t_count) {
		m_max_count = t_count;
		m_blobs.clear();
		init();
	}
	
	size_t ParticleSystem::get_real_count() const {
		return m_blobs.size()*m_blob_max_count;
	}
	
	size_t ParticleSystem::get_blob_count() const {
		return m_blobs.size();
	}
	
	void ParticleSystem::update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view) {		
		
		Vec3 position_buffer[m_blob_max_count];
		Color color_buffer[m_blob_max_count];

		const int dt = static_cast<int>(t_delta_time*1000.f);

		auto update_particle = [&](Particle* p, const size_t t_index) {
			if(p == nullptr) { return; }
			if(m_update_callback != nullptr) {
				p->lifetime += dt;
				p->dead = (p->lifetime >= p->max_lifetime);
				m_update_callback(p);
			
				position_buffer[t_index] = p->pos;
				color_buffer[t_index] = p->color;
			}
		};

		for(size_t i = 0; i < m_blobs.size(); i++) {
			Blob& b = m_blobs[i];
			
			for(size_t j = 0; j < b.particles.size(); j++) {
				update_particle(&b.particles[j], j);
			}

			b.shape_array.fill_point_pos(position_buffer, m_blob_max_count);
			b.shape_array.fill_colors((Color*)color_buffer, m_blob_max_count);
		}

		// render all particles
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		
		for(size_t i = m_continue_index; i < m_blobs.size(); i++) {
			m_timer.reset();
			m_blobs[i].shape_array.draw(t_shader);
			while(m_timer.elapsed_ms() < 1) {} // make sure its not rendering too fast
		}
	
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	void ParticleSystem::update_callback(void(*t_callback)(Particle*)) {
		m_update_callback = t_callback;
	}


}


