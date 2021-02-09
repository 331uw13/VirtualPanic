#pragma once
#include <GL/gl3w.h>
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
		~Shader();

		void load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings = 0);
		void unload();
		bool is_loaded() const;

		void use() const;
		
		// TODO: make sure that shader has uniform 't_name'

		void set_color  (const char* t_name, const Color& t_color)   const;
		void set_vec3   (const char* t_name, const glm::vec3& t_v3)  const;
		void set_vec2   (const char* t_name, const glm::vec2& t_v2)  const;
		void set_mat4   (const char* t_name, const glm::mat4& t_m)   const;
		void set_int    (const char* t_name, const int t_i)    const;
		void set_float  (const char* t_name, const float t_f)  const;
		
	private:

		void _compile_shaders();
		bool _shader_ok(const int t_id);
		void _read_sources(const char* t_filename, std::string& t_src_ref);
		void _safe_check_vertex_source(const uint32_t t_glsl_version);

		std::string m_vertex_source    { "" };
		std::string m_fragment_source  { "" };

		uint32_t m_id { 0 };
		bool m_loaded { false };
	
		// this adds things to the shader code
		// vpanic_light(), vpanic_fog()... they just help you calculate stuff if you are feeling lazy
		// (this is done by default so use NO_SHADER_UTIL when loading the shader if you dont want this)
		void _add_functions(const uint32_t t_glsl_version);

	};
}


