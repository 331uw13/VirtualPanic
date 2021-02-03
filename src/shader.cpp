#include <GL/glew.h>
#include <fstream>

#include "shader.hpp"
#include "messages.hpp"
#include "timer.hpp"

namespace VPanic {

	void Shader::load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings) {

		Timer timer;

		// TODO: check if the any source is changed, if it is then delete and load else gtfo
		if(m_loaded) {
		   	del();
	   	}

		if(!(t_settings & NO_SHADER_UTIL)) {
			_add_functions(t_glsl_version);
		}
		
		_safe_check_vertex_source(t_glsl_version);
		_read_sources(t_shader_filename, m_fragment_source);
	
		// TODO: 
		// add option for geometry shader
		// add option to support all kind of things in vertex shader
	
		printf("%s\n", m_fragment_source.c_str());

		_compile_shaders();	
		message(MType::DEBUG, "Shader::load() --> [%ims]", timer.elapsed());
	}

	void Shader::set_color(const char* t_name, const Color& t_color) const {
		glUniform4f(glGetUniformLocation(m_id, t_name), 
				t_color.r/255.0f, 
				t_color.g/255.0f,
			   	t_color.b/255.0f,
			   	t_color.a/255.0f);
	}
	
	void Shader::set_vec3(const char* t_name, const glm::vec3& t_v3) const {
		glUniform3f(glGetUniformLocation(m_id, t_name), t_v3.x, t_v3.y, t_v3.z);
	}
	
	void Shader::set_vec2(const char* t_name, const glm::vec2& t_v2) const {
		glUniform2f(glGetUniformLocation(m_id, t_name), t_v2.x, t_v2.y);
	}

	void Shader::set_mat4(const char* t_name, const glm::mat4& t_m) const {
		glUniformMatrix4fv(glGetUniformLocation(m_id, t_name), 1, GL_FALSE, &t_m[0][0]);
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

	void Shader::_compile_shaders() {
		const char* const vertex_src = m_vertex_source.c_str();
		const char* const fragment_src = m_fragment_source.c_str();

		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_src, NULL);
		glCompileShader(vertex_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(vertex_shader)) { 
			message(MType::BAD, "Vertex shader failed");
			return;
	   	}

		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_src, NULL);
		glCompileShader(fragment_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(fragment_shader)) {
			message(MType::BAD, "Fragment shader failed");
			return;
	   	}
		
		// create and link the shader program
		int program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);
		
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		
		int linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(linked == 0) {
			char msg[1024];
			glGetProgramInfoLog(program, 1024, NULL, msg);
			message(MType::BAD, "%1(Shader Program Link Error):%0 %s", msg);
			m_id = program;
			m_loaded = false;
			return;
		}
		m_id = program;
		m_loaded = true;
	}
	
	void Shader::_read_sources(const char* t_filename, std::string& t_src_ref) {	
		std::ifstream f;
		std::string full, line;
		f.open(t_filename);
		if(!f.is_open()) {
			message(MType::BAD, "Failed to open file: \"%s\"!", t_filename);
			t_src_ref = {};
		}
		while(getline(f, line)) {
			t_src_ref += line + '\n';
	   	}
		f.close();
	}

	bool Shader::_shader_ok(const int t_id) {
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
	
	
	void Shader::_add_functions(const uint32_t t_glsl_version) {
		m_fragment_source = 
			"#version " + std::to_string(t_glsl_version) +
			"\n"
			"in vec3 i_fg_pos;\n"
			"in vec3 i_normal;\n"
			"uniform vec4 u_shape_color;\n"
			"uniform vec3 u_camera_pos;\n"
			"vec3 vpanic_light(vec3 pos, vec4 color, float brightness, float ambient, float specular, float shine) {\n"
				" vec3 am = ambient*vec3(color)*brightness;\n"
				" vec3 norm = normalize(i_normal);\n"
				" vec3 ld = normalize(pos-i_fg_pos);\n"
				" vec3 rd = reflect(-ld, norm);\n"
				" vec3 diff = (max(dot(norm, ld), 0.0)*brightness)*vec3(color);\n"
				" float spec = pow(max(dot(normalize(u_camera_pos-i_fg_pos), rd), 0.0), shine);\n"
				" return (am+diff+spec)*vec3(u_shape_color);\n"
			"}";
	}
	
	void Shader::_safe_check_vertex_source(const uint32_t t_glsl_version) {
		if(m_vertex_source.size() <= 1 && m_vertex_source.find('#') != 0) {	
			m_vertex_source = 
				"#version " + std::to_string(t_glsl_version) +
				"\n"
				"in vec3 pos;\n"
				"in vec3 normal;\n"
				"out vec3 i_fg_pos;\n"
				"out vec3 i_normal;\n"
				"uniform mat4 proj;\n"
				"uniform mat4 view;\n"
				"uniform mat4 model;\n"
				"void main() {\n"
				"i_normal = normal;\n"
				"i_fg_pos = vec3(model*vec4(pos, 1.0));\n"
				"gl_Position = proj*view*model*vec4(pos, 1.0);\n"
				"}";
		}
	}
}











