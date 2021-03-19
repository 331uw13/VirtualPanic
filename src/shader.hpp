#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "vec3.hpp"
#include "vec2.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "light.hpp"
#include "settings.hpp"


namespace vpanic {

	class Shader {
	public:
		
		Shader() {}
		~Shader();

		// NOTE:
		// -  bool is_file_changed();
		// -  void update_source();
		// these could be helpful too! :)


		// you get control over fragment shader only and there are built in things to help you
		// you dont have access to vertex shader this way
		void load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings = 0);
		void load_from_memory(const char* t_shader_src, const uint32_t t_glsl_version, const int t_settings = 0);
		
		// same as one above but with geometry shader
		void load_with_geometry_shader(const char* t_shader_filename, const char* t_g_shader_filename, const uint32_t t_glsl_version, const int t_settings = 0);
		void load_with_geometry_shader_from_memory(const char* t_shader_src, const char* t_g_shader_src, const uint32_t t_glsl_version, const int t_settings = 0);
		
		// lets you do more things
		// see wiki how to set up vertex shader with this: <link to wiki page when its completed>
		void load(const char* t_vertex_filename, const char* t_fragment_filename);
		void load_from_memory(const char* t_vertex_src, const char* t_fragment_src);
	

		void unload();
		bool is_loaded() const;

		void use() const;
		void add_uniform_binding(const char* t_name);

		// TODO: make sure that shader has uniform 't_name'
		
		void set_color  (const char* t_name, const Color& t_color)   const;
		void set_vec3   (const char* t_name, const Vec3& t_v3)       const;
		void set_vec2   (const char* t_name, const Vec2& t_v2)       const;
		void set_mat4   (const char* t_name, const Matrix& t_value)  const;
		void set_int    (const char* t_name, const int t_value)      const;
		void set_bool   (const char* t_name, const bool t_value)     const; // you can just use set_int but i think this looks bit more clear :)
		void set_float  (const char* t_name, const float t_value)    const;

		uint32_t id { 0 };
		
	private:

		mutable std::unordered_map<const char*, int> m_saved;

		void _shader_init(const int t_settings, const uint32_t t_glsl_version);
		void _shader_compile_and_clear();
		void _compile_shaders();
		bool _shader_ok(const int t_id);
		void _read_sources(const char* t_filename, std::string& t_src_ref);
		void _safe_check_vertex_source(const uint32_t t_glsl_version);
		int _retrieve_location(const char* t_name) const;

		std::string m_vertex_source    { "" };
		std::string m_fragment_source  { "" };
		std::string m_geometry_source  { "" };

		bool m_has_geometry_shader { false };
		bool m_loaded { false };
		uint32_t m_bind_count { 0 };
	
		// this adds things to the shader code
		// vpanic_light(), vpanic_fog()... they just help you calculate stuff if you are feeling lazy
		// (this is done by default so use NO_SHADER_UTIL when loading the shader if you dont want this
		// or pass vertex shader to load function too, see above)
		void _add_functions(const uint32_t t_glsl_version);

	};
}


