#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "shape_array.hpp"
#include "timer.hpp"


/*

NOTES:


	-  drag to origin
	-  drag force

	-  velocity
	-  lifetime
	-  gravity
	-  colors
	-  max lifetime



*/
namespace vpanic {

	class ParticleSystem {
	public:

		// NOTE:
		// tests ?
		// for example: user can call particle_system.test_update(); to identify some problems ?
		// .. idk if this is very useful

		void init();
		void unload();
		
		void update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view);
		void resize(const uint32_t t_count);

		Vec3 origin { Vec3(0.0f, 2.0f, 0.0f) };

	private:
		
		uint32_t m_vao { 0 };
		uint32_t m_ssbo { 0 };
		Shader m_compute_shader;
	
		uint32_t m_max_count { 10000 };
		bool m_loaded { false };

		struct Particle {
			Vec4 pos;
			Vec4 velocity;
		};
	};

}
