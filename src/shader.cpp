#include <GL/gl3w.h>
#include <fstream>

#include "shader.hpp"
#include "messages.hpp"
#include "timer.hpp"

namespace vpanic {

	Shader::~Shader() {
		unload();
		message(MType::DEBUG, "Deleted shader");
	}

	void Shader::load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings) {

		Timer timer;

		// TODO: check if the any source is changed, if it is then delete and load else gtfo
		if(m_loaded) {
		   	unload();
	   	}

		if(!(t_settings & NO_SHADER_UTIL)) {
			_add_functions(t_glsl_version);
		}
		
		_safe_check_vertex_source(t_glsl_version);
		_read_sources(t_shader_filename, m_fragment_source);
	
		// TODO: 
		// add option for geometry shader
		// add option to support all kind of things in vertex shader

		_compile_shaders();
		
		if(m_loaded) {
			message(MType::OK, "Loaded shader %3\"%s\" %5[%ims]", t_shader_filename, timer.elapsed_ms());
		}

		// not needed anymore
		m_vertex_source.clear();
		m_fragment_source.clear();
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
	
	void Shader::set_int(const char* t_name, const int t_i) const {
		glUniform1i(glGetUniformLocation(m_id, t_name), t_i);
	}
	
	void Shader::set_float(const char* t_name, const float t_f) const {
		glUniform1f(glGetUniformLocation(m_id, t_name), t_f);		
	}

	bool Shader::is_loaded() const {
		return m_loaded;
	}
	
	void Shader::use() const {
		glUseProgram(m_id);
	}

	void Shader::unload() {
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
		
			// TODO: show part of source where the error is

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
			"#version " + std::to_string(t_glsl_version) + " core\n"
			"struct Fragment {\n"
				" vec3 pos;\n"
				" vec3 normal;\n"
				" vec2 texcoord;\n"
			"};\n"
			"struct VPanicCamera {\n"
				" vec3 pos;"
				" vec3 front;"
			"};\n"
			"struct VPanicShape {\n"
				" vec3 pos;"
				" vec4 color;"
			"};\n"
			"in Fragment fragment;\n"
			"uniform VPanicShape shape;"
			"uniform VPanicCamera camera;"	
			"uniform sampler2D texture0;" // NOTE: array of textures?
			"vec3 vpanic_light(vec3 pos, vec4 color, float brightness, float radius) {\n"
				" if(fragment.normal == vec3(0.0)) { return vec3(shape.color); }"
				" vec3 ambient = 0.15*vec3(color)*vec3(shape.color);"
				" vec3 norm = normalize(fragment.normal);"
				" vec3 light_dir = normalize(pos - fragment.pos);"
				" float diff = max(dot(norm, light_dir), 0.0);"
				" vec3 diffuse = diff*vec3(color);"
				" vec3 view_dir = normalize(camera.pos - fragment.pos);"
				//" vec3 reflect_dir = reflect(-light_dir, norm);"
				" vec3 half_dir = normalize(view_dir+light_dir);"
				" float spec = pow(max(dot(view_dir, half_dir), 0.0), 26.0);"
				" vec3 specular = spec*vec3(shape.color);"
				" float d = length(pos - fragment.pos);"
				" float att = smoothstep(radius+d, 0.0, d)*normalize(brightness);"
				" ambient *= att;"
				" diffuse *= att;"
				" specular *= att;"
				" vec3 res = vec3((ambient*brightness)+diffuse+specular);\n"
				" return vec3(1.0)-exp(-(res*vec3(shape.color)*brightness));"
			"}"
			"vec3 vpanic_directional_light(vec3 direction, vec4 color, float ambient_value, float diffuse_value, float specular_value) {\n"
				" if(fragment.normal == vec3(0.0)) { return vec3(shape.color); }"
				" vec3 ambient = ambient_value*vec3(color);"
				" vec3 norm = normalize(fragment.normal);"
				" vec3 light_dir = normalize(-direction);"
				" float diff = max(dot(norm, light_dir), 0.0);"
				" vec3 diffuse = diffuse_value*diff*vec3(color);"
				" vec3 view_dir = normalize(camera.pos - fragment.pos);"
				" vec3 reflect_dir = reflect(-light_dir, norm);"
				" float spec = pow(max(dot(view_dir, reflect_dir), 1.0), 72);"
				" vec3 specular = specular_value*spec*vec3(shape.color);"
				" return (ambient+diffuse+specular)*vec3(shape.color);\n"
			"}"
			"float vpanic_fog(vec3 pos, float max, float min) {\n"
				" float dist = distance(pos, fragment.pos);"
				" if(dist <= min) { return 0.0; }"
				" if(dist >= max) { return 1.0; }"
				" return 1.0 - (max - dist) / (max - min);"
			"}";
	}
	
	void Shader::_safe_check_vertex_source(const uint32_t t_glsl_version) {
		// make sure that vertex source is not empty and it starts with '#' else update it
		if(m_vertex_source.size() <= 1 && m_vertex_source.find(0x23) != 0) {	
			m_vertex_source = 
				"#version " + std::to_string(t_glsl_version) + " core\n"
				"layout(location = 0) in vec3 pos;\n"
				"layout(location = 1) in vec3 normal;\n"
				"layout(location = 2) in vec2 texcoord;\n"
				"layout(location = 3) in mat4 offset;\n"
				"struct Fragment {\n"
					" vec3 pos;\n"
					" vec3 normal;\n"
					" vec2 texcoord;\n"
				"};\n"
				"out Fragment fragment;\n"
				"uniform mat4 proj;\n"
				"uniform mat4 view;\n"
				"uniform mat4 model;\n"
				"uniform bool use_offset;\n"
				"void main() {\n"
					" fragment.normal = normal;\n"
					" fragment.texcoord = texcoord;\n"
					" fragment.pos = vec3(((use_offset) ? offset : model)*vec4(pos, 1.0));\n"
					" gl_Position = proj*view*((use_offset) ? offset : model)*vec4(pos, 1.0);\n"
				"}";
		}
	}
}











