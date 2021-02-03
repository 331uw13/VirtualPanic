#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>

#include "color.hpp"
#include "light.hpp"
#include "settings.hpp"

namespace VPanic {

	class Shader {
	public:
		
		Shader() {}
		~Shader() { del(); }

		void load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings = 0);


		// TODO: make sure that shader has uniform 't_name'

		void set_color(const char* t_name, const Color& t_color) const;
		void set_vec3(const char* t_name, const glm::vec3& t_v3) const;
		void set_vec2(const char* t_name, const glm::vec2& t_v2) const;
		void set_mat4(const char* t_name, const glm::mat4& t_m) const;
	
		// NOTE: this is stupid!
		template<typename T> void set_value(const char* t_name, const T& v) const {
			glUniform1f(glGetUniformLocation(m_id, t_name), v);
		}

		void use() const;
		void del();
		
		bool is_loaded() const;
		int get_id() const;

	private:

		void _compile_shaders();
		bool _shader_ok(const int t_id);
		void _read_sources(const char* t_filename, std::string& t_src_ref);
		void _safe_check_vertex_source(const uint32_t t_glsl_version);

		std::string m_vertex_source;
		std::string m_fragment_source;


		bool m_loaded { false };
		int m_id { 0 };
	
		// this adds things to the shader code
		// vpanic_light(), vpanic_fog()... they just help you calculate stuff if you are feeling lazy
		// (this is done by default. Use NO_SHADER_UTIL when loading the shader if you dont want this)
		void _add_functions(const uint32_t t_glsl_version);

	};
}


