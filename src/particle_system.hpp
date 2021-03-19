#pragma once
#include <vector>

#include "vec3.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "shape_array.hpp"
#include "timer.hpp"

// this is probably going to change a bit


namespace vpanic {

	struct Particle {
	
		Vec3 pos              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 vel              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 acc              { Vec3(0.0f, 0.0f, 0.0f) };
		Vec3 scale            { Vec3(0.4f, 0.4f, 0.4f) };
		uint32_t lifetime      { 0 };
		uint32_t max_lifetime  { 1000 };
		bool dead              { true };
		Color color            { Color(255, 255, 255) };
		Matrix matrix          { Matrix(1.0f) };

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
		void resize(const uint32_t t_count);
		size_t get_real_count() const;
		size_t get_blob_count() const;

	private:
		
		struct Blob {
			std::vector<Particle> particles;
			ShapeArray shape_array;
		};
		
		uint32_t m_blob_max_count { 10000 };
		uint32_t m_max_count { 5000 };
		
		std::vector<Blob> m_blobs;
		Timer m_timer;
		uint32_t m_continue_index { 0 };
		void(*m_update_callback)(Particle*) { nullptr };

		void _update_blob(const Blob& b, Matrix* t_matrix_buffer, Color* t_color_buffer, const float t_delta_time, const Matrix& t_view);
	
	};

}
