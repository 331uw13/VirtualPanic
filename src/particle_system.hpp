#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "shape_array.hpp"
#include "timer.hpp"


namespace vpanic {

	class ParticleSystem {
	public:

		void init();
		void unload();
		
		void resize(const uint32_t t_count);
		void update(const Shader& t_shader, const Shader& t_compute_shader, const float t_delta_time);

		Vec3 origin { Vec3(0.0f, 2.0f, 0.0f) };

	private:
		
		uint32_t m_vao { 0 };
		uint32_t m_ssbo { 0 };
	
		uint32_t m_max_count { 2000 };
		bool m_loaded { false };
	};

}
