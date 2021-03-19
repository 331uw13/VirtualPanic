#include <fstream>
#include "libs/gl3w.h"

#include "shader.hpp"
#include "messages.hpp"


namespace vpanic {

	Shader::~Shader() {
		unload();
	}

	void Shader::_shader_init(const int t_settings, const uint32_t t_glsl_version) {
		
		// TODO: check if the any source is changed, if it is then delete and load else just return
		if(m_loaded) {
			unload();
		}

		if(t_settings != NO_SHADER_UTIL) {
			_add_functions(t_glsl_version);
		}
	}
	
	void Shader::_shader_compile_and_clear() {
		_compile_shaders();
		m_vertex_source.clear();
		m_fragment_source.clear();
	}

	void Shader::load(const char* t_shader_filename, const uint32_t t_glsl_version, const int t_settings) {
		_shader_init(t_settings, t_glsl_version);
		_safe_check_vertex_source(t_glsl_version);
		_read_sources(t_shader_filename, m_fragment_source);
		_shader_compile_and_clear();
	}
	
	void Shader::load_from_memory(const char* t_shader_src, const uint32_t t_glsl_version, const int t_settings) {
		_shader_init(t_settings, t_glsl_version);
		_safe_check_vertex_source(t_glsl_version);
		m_fragment_source = t_shader_src;
		_shader_compile_and_clear();
	}

	void Shader::load_with_geometry_shader(const char* t_shader_filename, const char* t_g_shader_filename, const uint32_t t_glsl_version, const int t_settings) {
		m_has_geometry_shader = true;
		_shader_init(t_settings, t_glsl_version);
		_safe_check_vertex_source(t_glsl_version);
		_read_sources(t_shader_filename, m_fragment_source);
		_read_sources(t_g_shader_filename, m_geometry_source);
		_shader_compile_and_clear();
	}

	void Shader::load_with_geometry_shader_from_memory(const char* t_shader_src, const char* t_g_shader_src, const uint32_t t_glsl_version, const int t_settings) {
		m_has_geometry_shader = true;
		_shader_init(t_settings, t_glsl_version);
		_safe_check_vertex_source(t_glsl_version);
		m_fragment_source = t_shader_src;
		m_geometry_source = t_g_shader_src;
		_shader_compile_and_clear();
	}
	
	void Shader::load(const char* t_vertex_filename, const char* t_fragment_filename) {
		_shader_init(0, 0);
		_read_sources(t_vertex_filename, m_vertex_source);
		_read_sources(t_fragment_filename, m_fragment_source);
		_shader_compile_and_clear();
	}
	
	void Shader::load_from_memory(const char* t_vertex_src, const char* t_fragment_src) {
		_shader_init(0, 0);
		m_vertex_source = t_vertex_src;
		m_fragment_source = t_fragment_src;
		_shader_compile_and_clear();
	}

	void Shader::set_color(const char* t_name, const Color& t_color) const {
		glUniform4f(glGetUniformLocation(id, t_name), 
				t_color.r/255.0f, 
				t_color.g/255.0f,
			   	t_color.b/255.0f,
			   	t_color.a/255.0f);
	}
	
	int Shader::_retrieve_location(const char* t_name) const {
		// if name is not found in saved names then ask opengl if it can find it
		// if not then no need to do anything
		// but if its found then add it to saved names
		if(m_saved.find(t_name) == m_saved.end()) {
			const int l = glGetUniformLocation(id, t_name);
			if(l >= 0) {
				m_saved[t_name] = l;
			}
			return l;
		}
		else {
			return m_saved[t_name];
		}
		return -1;
	}
	
	void Shader::set_vec3(const char* t_name, const Vec3& t_v3) const {
		glUniform3f(_retrieve_location(t_name), t_v3.x, t_v3.y, t_v3.z);
	}
	
	void Shader::set_vec2(const char* t_name, const Vec2& t_v2) const {
		glUniform2f(_retrieve_location(t_name), t_v2.x, t_v2.y);
	}

	void Shader::set_mat4(const char* t_name, const Matrix& t_value) const {
		glUniformMatrix4fv(_retrieve_location(t_name), 1, GL_FALSE, t_value.begin());
	}
	
	void Shader::set_int(const char* t_name, const int t_value) const {
		glUniform1i(_retrieve_location(t_name), t_value);
	}
	
	void Shader::set_bool(const char* t_name, const bool t_value) const {
		set_int(t_name, t_value);
	}
	
	void Shader::set_float(const char* t_name, const float t_value) const {
		glUniform1f(_retrieve_location(t_name), t_value);
	}

	bool Shader::is_loaded() const {
		return m_loaded;
	}
	
	void Shader::use() const {
		glUseProgram(id);
	}
	
	void Shader::add_uniform_binding(const char* t_name) {
		if(!m_loaded) { return; }
		const uint32_t block_index = glGetUniformBlockIndex(id, t_name);
		glUniformBlockBinding(id, block_index, m_bind_count);
		m_bind_count++;
	}

	void Shader::unload() {
		if(!m_loaded) { return; }
		glDeleteProgram(id);
		id = 0;
		m_loaded = false;
		m_has_geometry_shader = false;
	}

	void Shader::_compile_shaders() {
		const char* const vertex_src = m_vertex_source.c_str();
		const char* const fragment_src = m_fragment_source.c_str();

		int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_src, NULL);
		glCompileShader(vertex_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(vertex_shader)) { 
			message(MType::ERROR, "Vertex shader failed");
			return;
	   	}

		int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_src, NULL);
		glCompileShader(fragment_shader);
		
		// check if shader has been compiled succesfully
		if(!_shader_ok(fragment_shader)) {
			message(MType::ERROR, "Fragment shader failed");
			return;
	   	}
		
		// check if user wanted geometry shader
		int geometry_shader = 0;
		const bool with_gs = (m_has_geometry_shader);
		if(with_gs) {

			message(MType::DEBUG, "Using geometry shader");

			geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
			const char* const g_src = m_geometry_source.c_str();
			glShaderSource(geometry_shader, 1, &g_src, NULL);
			glCompileShader(geometry_shader);
	
			// aaand make sure that it has been compiled succesfully
			if(!_shader_ok(geometry_shader)) {
				message(MType::ERROR, "Geometry shader failed");
				return;
			}

			message(MType::DEBUG, "Geometry shader compiled succesfulalsygisayfaig00g");
		}

		// create and link the program
		int program = glCreateProgram();
		
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		if(with_gs) {
			glAttachShader(program, geometry_shader);
		}

		glLinkProgram(program);
		
		// they are compiled and linked to the program so they can be deleted
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		if(with_gs) {
			glDeleteShader(geometry_shader);
		}

		int linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(linked == 0) {
			char msg[1024];
			glGetProgramInfoLog(program, 1024, NULL, msg);
			message(MType::ERROR, "(Shader Program Link Error): %s", msg);
			id = program;
			m_loaded = false;
			return;
		}
		id = program;
		m_loaded = true;
	}
	
	void Shader::_read_sources(const char* t_filename, std::string& t_src_ref) {	
		std::ifstream f;
		std::string full, line;
		f.open(t_filename);
		if(!f.is_open()) {
			message(MType::ERROR, "Failed to open file: \"%s\"!", t_filename);
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
			message(MType::ERROR, "(Shader Compile Error): %s", msg);
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
				" vec3 texcoord3d;\n"
			"};\n"
			"struct VPanicShape {\n"
				" vec3 pos;"
				" vec4 color;"
			"};\n"
			
			"in vec3 camera_pos;"
			"in Fragment fragment;\n"
			"in VPanicShape shape;\n"
			"uniform sampler2D texture0;" // NOTE: array of textures?
			
			"vec3 vpanic_light(vec3 pos, vec4 color, float brightness, float radius) {\n"
				" if(fragment.normal == vec3(0.0f, 0.0f, 0.0f)) { return vec3(shape.color); }"
				" vec3 light_color = vec3(color);"
				" vec3 shape_color = vec3(shape.color);"
				" vec3 light_dir = normalize(pos - fragment.pos);"
				" vec3 norm = normalize(fragment.normal);"
				" vec3 ambient = 0.4f*light_color;" // TODO
				" float diff = max(dot(norm, light_dir), 0.0f);"
				" vec3 diffuse = 0.5f*diff*light_color;"
				" vec3 view_dir = normalize(camera_pos-fragment.pos);"
				" float spec = pow(max(dot(norm, normalize(light_dir+view_dir)), 0.0f), 64.0f);"
				" vec3 specular = 0.34f*spec*light_color;"
				" float d = length(pos-fragment.pos);"
				" float att = smoothstep(radius+d, 0.0, d);"
				" vec3 res = vec3((ambient*att)+(diffuse*att)+(specular*att))*shape_color;"
				" return vec3(1.0)-exp(-(res*shape_color*brightness));"
			"}"
			"vec3 vpanic_directional_light(vec3 direction, vec4 color, float ambient_value, float diffuse_value, float specular_value) {\n"
				" if(fragment.normal == vec3(0.0f, 0.0f, 0.0f)) { return vec3(shape.color); }"
				" vec3 light_color = vec3(color);"
				" vec3 shape_color = vec3(shape.color);"
				" vec3 light_dir = normalize(direction);"
				" vec3 view_dir = normalize(camera_pos-fragment.pos);"
				" vec3 norm = normalize(fragment.normal);"
				" vec3 ambient = ambient_value*light_color;" // TODO
				" float diff = max(dot(norm, light_dir), 0.0f);"
				" vec3 diffuse = diffuse_value*diff*light_color;"
				" float spec = pow(max(dot(norm, normalize(light_dir+view_dir)), 0.0f), 64.0f);"
				" vec3 specular = spec*light_color;"
				" return (ambient+diffuse+specular)*shape_color;"
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
		if(m_vertex_source.size() <= 5 && m_vertex_source.find('#') != 0) {	
			m_vertex_source = 
				"#version " + std::to_string(t_glsl_version) + " core\n"
				"layout(location = 0) in vec3 pos;\n"
				"layout(location = 1) in vec3 normal;\n"
				"layout(location = 2) in vec2 texcoord;\n"

				"layout(location = 3) in vec4 offset_color;\n"
				"layout(location = 4) in vec3 particle_pos;\n"
				"layout(location = 5) in mat4 offset;\n"

				"struct Fragment {"
					" vec3 pos;"
					" vec3 normal;"
					" vec2 texcoord;"
					" vec3 texcoord3d;"
				"};"
				"layout (std140) uniform vertex_data {"
					" uniform mat4 matrix;"
					" uniform vec3 cam_pos;"
				"};"
				"struct VPanicShape {"
					" vec3 pos;"
					" vec4 color;"
				"};"
				
				"uniform mat4 model;"
				"uniform vec4 shape_color;"
				"uniform vec3 shape_pos;"

				"uniform int mode;"

				"out vec3 camera_pos;"
				"out Fragment fragment;"
				"out VPanicShape shape;"
				
				"void main() {"
					" camera_pos = cam_pos;"
					" fragment.texcoord = texcoord;"
					" fragment.texcoord3d = pos;"

					"switch(mode) {"

						"case 0:" // shader_mode__shape
							"shape.pos = shape_pos;"
							"shape.color = shape_color;"
							"fragment.normal = mat3(transpose(inverse(model)))*normal;"
							"fragment.pos = vec3(model*vec4(pos, 1.0));"
							"gl_Position = matrix*model*vec4(pos, 1.0);"
						"break;"

						"case 1:" // shader_mode__shape_array
							"shape.pos = offset[3].xyz;"
							"shape.color = offset_color;"
							"fragment.normal = mat3(transpose(inverse(offset)))*normal;"
							"fragment.pos = vec3(offset*vec4(pos, 1.0));"
							"gl_Position = matrix*offset*vec4(pos, 1.0);"
						"break;"
						
						"case 2:" // shader_mode__shape_array_point
							"shape.pos = particle_pos;"
							"shape.color = offset_color;"
							"fragment.pos = particle_pos;"
							"fragment.normal = vec3(0.0f, 0.0f, 0.0f);"
							"gl_Position = matrix*mat4(1.0f)*vec4(particle_pos, 1.0);"
						"break;"

						"default:break;"
					"}"

					/*
					" if(mode == 2) {"
						" shape.pos = offset[3].xyz;"
						" shape.color = offset_color;"
						" fragment.normal = mat3(transpose(inverse(offset)))*normal;"
						" fragment.pos = vec3(offset*vec4(pos, 1.0));\n"
						" gl_Position = matrix*offset*vec4(pos, 1.0);\n"
					" }"
					" else if(mode == 1){"
						" shape.pos = shape_pos;"
						" shape.color = shape_color;"
						" fragment.normal = mat3(transpose(inverse(model)))*normal;"
						" fragment.pos = vec3(model*vec4(pos, 1.0));\n"
						" gl_Position = matrix*model*vec4(pos, 1.0);\n"
					" }"
					*/
				"}";
		}
	}
}











