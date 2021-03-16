#include <cmath>
#include "libs/gl3w.h"


#include "particle_system.hpp"
#include "utils.hpp"


namespace vpanic {

	void ParticleSystem::init() {
		std::vector<Vertex> data;
		add_plane_data(data);
		set_normals(data);

		m_blob_max_count = m_max_count/2;
		clamp<uint32_t>(m_blob_max_count, 1, 10000);

		const int blob_count = ceil(m_max_count/m_blob_max_count);
		m_blobs.resize(blob_count);

		for(size_t i = 0; i < m_blobs.size(); i++) {
			m_blobs[i].shape_array.load(data);
			m_blobs[i].shape_array.reserve(m_blob_max_count);
			m_blobs[i].particles.resize(m_blob_max_count);
		}

	}

	void ParticleSystem::resize(const uint32_t t_count) {
		m_max_count = t_count;
		m_blobs.clear();
		init();
	}
	
	size_t ParticleSystem::size() const {
		return m_max_count;
	}

	void ParticleSystem::update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view) {		
		
		Matrix matrix_buffer[m_blob_max_count];
		Vec4 color_buffer[m_blob_max_count];


		for(size_t i = 0; i < m_blobs.size(); i++) {
			Blob& b = m_blobs[i];

			for(size_t j = 0; j < b.particles.size(); j++) {
				Particle* p = &b.particles[j];
				p->lifetime += static_cast<uint32_t>(t_delta_time*1000.f);
				p->matrix.clear();
				
				if(m_update_callback != nullptr) {
					if(p->lifetime >= p->max_lifetime) {
						p->dead = true;
					}
					m_update_callback(p);
				}

				p->matrix.translate(p->pos);
				//p->matrix[3] = Vec4(p->pos.x, p->pos.y, p->pos.z, 0.0f);
				p->matrix.copy_rotation(t_camera_view);
				p->matrix.scale(p->scale);

				matrix_buffer[j] = p->matrix;
				color_buffer[j] = Vec4(p->color.r/255.f, p->color.g/255.f, p->color.b/255.f, p->color.a/255.f);
			}

			b.shape_array.place_matrices(matrix_buffer, m_blob_max_count);
			b.shape_array.place_colors(color_buffer, m_blob_max_count);
		}

		for(size_t i = m_continue_index; i < m_blobs.size(); i++) {
			m_timer.reset();
			glDepthMask(GL_FALSE);
			m_blobs[i].shape_array.draw(t_shader);
			glDepthMask(GL_TRUE);
			while(m_timer.elapsed_ms() < 2) {}
		}

		/*
		for(size_t i = 0; i < m_particles.size(); i++) {
			Particle* p = &m_particles[i];
			p->lifetime += static_cast<uint32_t>(t_delta_time*1000.f);
			if(p->lifetime >= p->max_lifetime) {
				p->dead = true;
			}
			if(m_update_callback != nullptr) {
				m_update_callback(p);
			}

			p->matrix.clear();
			p->matrix.translate(p->pos);
			p->matrix.copy_rotation(t_camera_view);
			p->matrix.scale(p->scale);

			// TODO: test performance between setting matrices like this or first updating private array of matrices and setting that?
			m_shape_array.set_matrix(i, p->matrix);
			m_shape_array.set_color(i, p->color);
		}

		glDepthMask(GL_FALSE);
		m_shape_array.draw(t_shader);
		glDepthMask(GL_TRUE);
		*/
	}
	
	void ParticleSystem::update_callback(void(*t_callback)(Particle*)) {
		m_update_callback = t_callback;
	}


}


