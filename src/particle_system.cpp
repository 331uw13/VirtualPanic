#include <cmath>
#include <thread>
#include "libs/gl3w.h"

#include "particle_system.hpp"
#include "utils.hpp"
#include "internal.hpp"
#include "messages.hpp"
#include "mouse.hpp"

#include "console.hpp" // delete this

// for testing
#define GROUP_SIZE_X 512


namespace vpanic {

	void ParticleSystem::init() {
	
		if(m_loaded) {
			unload();
		}

		// Use compute shader to compute point positions.
		// You could use geometry shader then to make them to whatever you like


		Particle particles[m_max_count];
		
		const float range = 1.5f;
		for(int i = 0; i < m_max_count; i++) {
			
			const float x = random(-range, range);
			const float y = random(0.0f, range*2);
			const float z = random(-range, range);
			//const float w = random(-range, range);

			const Vec4 p(x, y, z, 1.0f);

			particles[i].pos = p;
			particles[i].velocity = Vec4(0.0f);
		}

		glGenBuffers(1, &m_ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle)*m_max_count, particles, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
		m_compute_shader.add_shader("test_compute_shader.glsl", shadertype::compute);
		m_compute_shader.compile();

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	
		glBindBuffer(GL_ARRAY_BUFFER, m_ssbo);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Particle)*MAX_PARTICLES, particles, GL_STATIC_DRAW);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glEnableVertexAttribArray(6);
		
		m_loaded = true;
	}

	void ParticleSystem::resize(const uint32_t t_count) {
		m_max_count = t_count;
		if(!m_loaded) { return; }
	
		Particle particles[m_max_count];
		
		const float range = 1.5f;
		for(int i = 0; i < m_max_count; i++) {
			
			const float x = random(-range, range);
			const float y = random(0.0f, range*2);
			const float z = random(-range, range);
			//const float w = random(-range, range);

			const Vec4 p(x, y, z, 1.0f);

			particles[i].pos = p;
			particles[i].velocity = Vec4(0.0f);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle)*m_max_count, particles, GL_STATIC_DRAW);
	}
	
	void ParticleSystem::update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view) {		
		if(!m_loaded) { return; }
		m_compute_shader.use();
		m_compute_shader.set_int("max_particles", m_max_count);
		m_compute_shader.set_float("dt", t_delta_time);
		m_compute_shader.set_vec4("origin", Vec4(origin.x, origin.y, origin.z, 1.0f));

		glDispatchCompute((m_max_count/GROUP_SIZE_X)+1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		t_shader.use();
		t_shader.set_int("mode", internal::shader_mode__particle);

		glBlendFunc(GL_SRC_COLOR, GL_ONE);
		glBindVertexArray(m_vao);

		glDrawArraysInstanced(GL_POINTS, 0, m_max_count, 1);
		
		//glDrawArrays(GL_POINTS, 0, m_max_count);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	}
	
	void ParticleSystem::unload() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_ssbo);
		m_vao = 0;
		m_ssbo = 0;
		m_compute_shader.unload();
		m_loaded = false;
	}

}

