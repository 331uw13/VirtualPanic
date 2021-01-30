#include <GL/glew.h>
#include <fstream>

#include "shader.hpp"
#include "messages.hpp"

namespace VPanic {

	void Shader::load_shaders(const char* t_vertex_filename, const char* t_fragment_filename) {
		m_id = _compile_shader(_read_file(t_vertex_filename).c_str(), _read_file(t_fragment_filename).c_str());
	}

	void Shader::set_color(const char* t_name, const Color& t_color) const {
		glUniform4f(glGetUniformLocation(m_id, t_name), t_color.r, t_color.g, t_color.b, t_color.a);
	}
	
	void Shader::set_vec3(const char* t_name, const vec3& t_v3) const {
		glUniform3f(glGetUniformLocation(m_id, t_name), t_v3.x, t_v3.y, t_v3.z);
	}

	void Shader::set_mat4(const char* t_name, const glm::mat4& t_m) const {
		glUniformMatrix4fv(glGetUniformLocation(m_id, t_name), 1, GL_FALSE, &t_m[0][0]);
	}
	
	void Shader::set_float(const char* t_name, const float t_f) const {
		glUniform1f(glGetUniformLocation(m_id, t_name), t_f);
	}
	
	void Shader::set_bool(const char* t_name, const bool t_b) const {
		glUniform1f(glGetUniformLocation(m_id, t_name), t_b);
	}



	int Shader::get_id() const {
		return m_id;
	}
	bool Shader::is_loaded() const {
		return m_loaded;
	}
	
	void Shader::use() const {
		glUseProgram(m_id);
	}

	void Shader::del() {
		glDeleteProgram(m_id);
		m_id = 0;
		m_loaded = false;
	}

	int Shader::_compile_shader(const char* t_vertex_src, const char* t_fragment_src) {
		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &t_vertex_src, NULL);
		glCompileShader(vertex_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(vertex_shader)) { 
			message(MType::INFO, "vertex shader");
			return false;
	   	}

		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &t_fragment_src, NULL);
		glCompileShader(fragment_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(fragment_shader)) { 
			message(MType::INFO, "fragment shader");
			return false;
	   	}
		
		// create and link the shader program
		int program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);
		
		int linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(linked == 0) {
			char msg[1024];
			glGetProgramInfoLog(program, 1024, NULL, msg);
			message(MType::BAD, "%1(Shader Program Link Error):%0 %s", msg);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			m_loaded = false;
			return program;
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	
		m_loaded = true;
		return program;
	}
	
	std::string Shader::_read_file(const char* t_filename) {	
		std::ifstream f;
		std::string full, line;
		f.open(t_filename);
		if(!f.is_open()) {
			message(MType::BAD, "Failed to open file: \"%s\"", t_filename);
			return "";
		}
		while(getline(f, line)) { full += line + '\n'; }
		f.close();
		return full;
	}

	bool Shader::_shader_ok(int t_id) {
		int iv = 0;
		glGetShaderiv(t_id, GL_COMPILE_STATUS, &iv);
		if(iv == 0) {
			char msg[1024];
			glGetShaderInfoLog(t_id, 1024, NULL, msg);
			message(MType::BAD, "%1(Shader Compile Error):%0 %s", msg);
			m_loaded = false;
			return false;
		}
		return true;
	}
}
