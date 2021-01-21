#pragma once
#include "texture.hpp"
#include "engine.hpp"

namespace VPanic {

	// TODO:
	// - array of colors to fade
	// - mix color with nearby particles


	enum class ParticleType {
		NONE,
		TEXTURE,
		RECT,
		CIRCLE,
		TRAIL,
	};

	class ParticleSystem {
	public:

		void update(Engine* t_engine);
		void clear_particles();
		uint32_t particle_count();

		void add_texture(
				const Texture&  t_texture,
				const vec2&     t_pos,
				const vec2&     t_vel,
				const vec2&     t_acc,
				const Color&    t_color,
				int             t_max_lifetime
			);

		// TODO:
		// add_rect
		// add_circle
		// add_trail


		// negative value for "no limit"
		void max_particles(int t_max);
		
		void update_delay(uint32_t t_delay);
		void alpha_over_lifetime(bool t_b);
		void size_over_lifetime(bool t_b);
		
		// how much should the particle size grow over lifetime?
		// does nothing if size_by_lifetime is set to false
		void size_modifier(float t_mod);

		ParticleSystem() {}
		~ParticleSystem() { clear_particles(); }

	private:
	
		struct Particle {
			ParticleType type;
			vec2 pos;
			vec2 vel;
			vec2 acc;
			vec2 size;
			Color color;
			int max_lifetime;
			int lifetime;

			union {
				Texture texture;
				// ...
			};
			~Particle() {}
		};

		bool _can_add();

		std::vector<Particle> m_particles;
		Timer m_timer { Timer() };
		
		bool m_alpha_over_lifetime  { true };
		bool m_size_over_lifetime   { false };
	
		uint32_t m_update_delay  { 5 }; 
		int m_max_particles { -1 };
		float m_size_mod { 0.03f };
	
		
	};
}



