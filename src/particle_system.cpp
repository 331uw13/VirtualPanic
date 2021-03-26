#include <cmath>
#include <cstring>
#include "libs/gl3w.h"

#include "particle_system.hpp"
#include "utils.hpp"
#include "internal.hpp"
#include "messages.hpp"
#include "state.hpp"
#include "mouse.hpp"


#define GROUP_SIZE_X 512


namespace vpanic {

	void ParticleSystem::init() {
	
		if(m_loaded) {
			unload();
		}


		// Use compute shader to compute point positions.
		// You could use geometry shader then to make them to whatever you like


		/*
			"A structure and each structure member have a base offset and a base alignment,
		   	from which an aligned offset is computed by rounding the base offset up to a multiple of the base alignment."
			"The base offset of the first member of a structure is taken from the aligned offset of the structure itself."

			https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf
			|_  "7.6.2.2    Standard Uniform Block Layout"
		
		*/
		
		
		struct Particle {
			
			Vec4 attr0; // used for position
			Vec4 attr1; // used for velocity
			Vec4 attr2; // used for color
			Vec3 attr3; // free
			float life; // lifetime

		};

		Particle part;
		part.attr0 = Vec4(origin.x, origin.y, origin.z, 1.0f);
		part.attr1 = Vec4(0.0f);
		part.attr2 = Vec4(0.0f);
		part.attr3 = Vec3(0.0f);
		part.life = 0.0f;

		const size_t size = m_max_count*sizeof(Particle);
		glGenBuffers(1, &m_ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);

		for(uint32_t i = 0; i < m_max_count; i++) {
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, i*sizeof(Particle), sizeof(Particle), &part);
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo);
		
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_ssbo);

		// position
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
		glEnableVertexAttribArray(9);
		
		// color
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(Vec4)*2));
		glEnableVertexAttribArray(10);
		
		m_loaded = true;
	}
	
	void ParticleSystem::resize(const uint32_t t_count) {
		m_max_count = t_count;
		if(!m_loaded) { return; }
		init(); // NOTE: optimize later...
	}

	void ParticleSystem::update(const Shader& t_shader, const Shader& t_compute_shader, const float t_delta_time) {		
		if(!m_loaded) { return; }
		//if(!t_compute_shader.copy_state()[ShaderState::HAS_CS]) { return; }
		
		t_compute_shader.use();
		t_compute_shader.set_int("max_particles", m_max_count);
		t_compute_shader.set_bool("mouse_left_down", mouse::button_down(mouse::LEFT));
		t_compute_shader.set_bool("mouse_right_down", mouse::button_down(mouse::RIGHT));
		t_compute_shader.set_vec4("origin", Vec4(origin.x, origin.y, origin.z, 1.0f));
		t_compute_shader.set_float("dt", t_delta_time);

		uint32_t group_size = m_max_count/GROUP_SIZE_X;

		glDispatchCompute(group_size+1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

		t_shader.use();
		t_shader.set_int("mode", internal::shader_mode__particle);

		glBindVertexArray(m_vao);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);
		glDrawArrays(GL_POINTS, 0, m_max_count);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
		t_shader.disable();
	}
	
	void ParticleSystem::unload() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_ssbo);
		m_vao = 0;
		m_ssbo = 0;
		m_loaded = false;
	}

}

