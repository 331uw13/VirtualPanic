#include "particle_system.hpp"
#include "utils.hpp"

namespace VPanic {

	void ParticleSystem::update(Engine* t_engine) {
		if(t_engine == nullptr) { return; }
		const uint32_t size = m_particles.size();
		if(size == 0) { return; }

		uint32_t elapsed = m_timer.elapsed();
		bool do_update = false;
		if((do_update = (elapsed > m_update_delay))) {
			m_timer.reset();
		}

		for(uint32_t i = 0; i < size; i++) {
			Particle* p = &m_particles[i];
			if(p == nullptr) { continue; }
			if(p->type == ParticleType::NONE) { continue; }
			
			if(do_update) {
				if(p->lifetime >= p->max_lifetime) {
					m_particles.erase(m_particles.begin() + i);
					continue;
				}
				p->lifetime += elapsed;
				p->pos += p->vel;
				p->vel += p->acc;

				int a = 255;	
				if(m_alpha_over_lifetime) {
					a = lerp(p->color.a, 0, norm(p->lifetime, 0, p->max_lifetime + m_update_delay));
					clamp<int>(a, 0, 255);
				}

				if(m_size_over_lifetime) {
					p->size += vec2(m_size_mod);
				}

				switch(p->type) {
					case ParticleType::TEXTURE:
						p->texture.scale(p->size);
						p->texture.colorize(copy_dim_color(p->color, a));
						p->texture.position(p->pos);
						break;

					case ParticleType::RECT:
						break;
					
					case ParticleType::CIRCLE:
						break;

					case ParticleType::TRAIL:
						break;
					
					default: break;
				}

			}
			switch(p->type) {
				case ParticleType::TEXTURE:
					p->texture.render(t_engine);
					break;

				case ParticleType::RECT:
					break;
				
				case ParticleType::CIRCLE:
					break;

				case ParticleType::TRAIL:
					break;
				
				default: break;
			}
		}
	}

	uint32_t ParticleSystem::particle_count() {
		return m_particles.size();
	}

	void ParticleSystem::clear_particles() {
		m_particles.clear();
	}

	void ParticleSystem::add_texture(
			const Texture&  t_texture,
			const vec2&     t_pos,
			const vec2&     t_vel,
			const vec2&     t_acc,
			const Color&    t_color,
			int             t_max_lifetime)
   	{
		if(!_can_add()) { return; }
		m_particles.push_back(Particle { ParticleType::TEXTURE,
				t_pos, t_vel, t_acc, t_texture.get_scale(), t_color, t_max_lifetime, 0, { t_texture }});

	}

	bool ParticleSystem::_can_add() {
		return !(m_max_particles > 0 && m_particles.size() < static_cast<uint32_t>(m_max_particles));
	}
	
	void ParticleSystem::max_particles(int t_max) {
		if(m_particles.size() > static_cast<uint32_t>(t_max)) {
			m_particles.erase(m_particles.begin(), m_particles.begin() + t_max);
		}
		m_max_particles = t_max;
	}

	void ParticleSystem::update_delay(uint32_t t_delay) {
		m_update_delay = t_delay;
		m_timer.reset();
	}

	void ParticleSystem::alpha_over_lifetime(bool t_b) {
		m_alpha_over_lifetime = t_b;
	}

	void ParticleSystem::size_over_lifetime(bool t_b) {
		m_size_over_lifetime = t_b;
	}
	
	void ParticleSystem::size_modifier(float t_mod) {
		if(m_size_over_lifetime) {
			m_size_mod = t_mod;
		}
	}


}








