#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "vec3.hpp"
#include "vec2.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "settings.hpp"
#include "state.hpp"


namespace vpanic {

	enum class shadertype {
		vertex = 0,
		fragment = 1,
		geometry = 2,
		compute = 3

			// TODO:  "particle shader" that is compute shader but used by particle system and it has already some functions like "random" and has the uniforms and stuff already there
	};

	class Shader {
	public:
		
		Shader() {}
		~Shader();

		// NOTE:
		// -  bool is_file_changed();
		// -  void update_source();
		// these could be helpful too! :)

		void add_shader(const char* t_filename, shadertype t_type);
		void add_shaders_from_memory(const char* t_vssrc, const char* t_fssrc);
		bool compile();

		void unload();
		bool is_loaded() const;

		// TODO: check if its compiled or not
		void use() const;
		void add_uniform_binding(const char* t_name);

		void set_color  (const char* t_name, const Color& t_color)   const;
		void set_vec4   (const char* t_name, const Vec4& t_v4)       const;
		void set_vec3   (const char* t_name, const Vec3& t_v3)       const;
		void set_vec2   (const char* t_name, const Vec2& t_v2)       const;
		void set_mat4   (const char* t_name, const Matrix& t_value)  const;
		void set_int    (const char* t_name, const int t_value)      const;
		void set_bool   (const char* t_name, const bool t_value)     const; // you can just use set_int but i think this looks bit more clear :)
		void set_float  (const char* t_name, const float t_value)    const;

		ShaderState copy_state();

		uint32_t id { 0 };
		
	private:

		mutable std::unordered_map<const char*, int> m_saved;
		int _retrieve_location(const char* t_name) const;
		

		bool _compile_shader(const int t_shader_id, const char* t_src);
		bool _shader_ok(const int t_id);
		void _read_sources(const char* t_filename, std::string& t_src_ref);
		void _safe_check_vertex_source();
		void _add_functions();
		void _cleanup();

		ShaderState m_state;
		std::string m_sources[5] {
			"",
			"",
			"",
			""
		};

		const char* m_filenames[5] {
			"",
			"",
			"",
			""
		};
		
		bool m_raw { false };
		bool m_loaded { false };
		uint32_t m_bind_count { 0 };
	

	};
}


