#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "vec3.hpp"
#include "vec2.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "shader_component.hpp"


namespace vpanic {

	class Shader {
	public:
		
		Shader() {}
		~Shader();

		// NOTE:
		// -  bool is_file_changed();
		// -  void update_source();
		// these could be helpful too! :)

		bool add_src(const char* t_filename, const int t_type);
		bool add_src_from_memory(const std::string& t_src, const int t_type);

		bool compile();

		void unload();
		bool is_loaded() const;


		// TODO: "attach_shader()" / "deattach_shader()"
		// pointer to "shader attachment"
		//  ...

		void use() const;
		void disable() const;
		void add_uniform_binding(const char* t_name, const uint8_t t_binding_point);

		// TODO: rename some stuff
		void set_color  (const char* t_name, const Color& t_color)   const;
		void set_vec4   (const char* t_name, const Vec4& t_v4)       const;
		void set_vec3   (const char* t_name, const Vec3& t_v3)       const;
		void set_vec2   (const char* t_name, const Vec2& t_v2)       const;
		void set_mat4   (const char* t_name, const Matrix& t_value)  const;
		void set_int    (const char* t_name, const int t_value)      const;
		void set_bool   (const char* t_name, const bool t_value)     const; // you can just use set_int but i think this looks bit more clear :)
		void set_float  (const char* t_name, const float t_value)    const;
		
		uint32_t id { 0 };

	private:

		ShaderComponent m_components[4];
		int m_type_bits { 0 };

		enum { 
			SHADER = 0,
			PROGRAM = 1
		};
	
		bool _succeeded(const int t_id, const int t_type);
		void _add_functions(std::string& t_src, const int t_type);
		void _clear_sources();
		
		mutable std::unordered_map<const char*, int> m_saved;
		int _retrieve_location(const char* t_name) const;
	
		bool m_raw { false };
		bool m_loaded { false };

	};
}


