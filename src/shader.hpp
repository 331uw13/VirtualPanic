#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "color.hpp"

namespace VPanic {

	class Shader {
	public:
		
		Shader() {}
		~Shader() { del(); } 

		void load(const char* t_vertex_filename, const char* t_fragment_filename);
		void load_from_memory(const char* t_vertex_src, const char* t_fragment_src);

		void set_color(const char* t_name, const Color& t_color) const;
		void set_vec3(const char* t_name, const glm::vec3& t_v3) const;
		void set_vec2(const char* t_name, const glm::vec2& t_v2) const;
		void set_mat4(const char* t_name, const glm::mat4& t_m) const;
		
		template<typename T> void set_value(const char* t_name, const T& v) {
			glUniform1f(glGetUniformLocation(m_id, t_name), v);
		}

		/*
		// note: use template?
		void set_float(const char* t_name, const float t_f) const;
		void set_bool(const char* t_name, const bool t_b) const;
		// ...
		*/
		void use() const;
		void del();
		
		bool is_loaded() const;
		int get_id() const;

	private:

		int _compile_shader(const char* t_vertex_src, const char* t_fragment_src);
		bool _shader_ok(int t_id);
		std::string _read_file(const char* t_filename);

		bool m_loaded { false };
		int m_id { 0 };
	};

}
