#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "shape_array.hpp"

// this is probably going to change a bit


namespace vpanic {

	struct Particle {
	
		Vec3 pos              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 vel              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 acc              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 scale            { Vec3(0.4f, 0.4f, 0.4f) };
		float rotation        { 0.0f };
		float lifetime        { 0.0f };
		float max_lifetime    { 0.0f };
		bool dead             { true };
		//Color color         { Color(255, 255, 255) };
		Matrix _matrix        { Matrix(1.0f) };
	};


	class ParticleSystem {
	public:

		// NOTE:
		// tests ?
		// for example: user can call particle_system.test_update(); to identify some problems ?
		// .. idk if this is very useful

		void init();
		void update(const Shader& t_shader, const float t_delta_time, const Matrix& t_camera_view);
		void update_callback(void(*t_callback)(Particle*));

	private:
	
		uint32_t m_max_count { 500 };
		std::vector<Particle> m_particles;
		ShapeArray m_shape_array;

		void(*m_update_callback)(Particle*) { nullptr };
	};

}
