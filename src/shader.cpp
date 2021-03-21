#include <fstream>
#include <cstring>
#include <numeric>
#include "libs/gl3w.h"

#include "shader.hpp"
#include "messages.hpp"
#include "timer.hpp"


namespace vpanic {

	Shader::~Shader() {
		unload();
	}
	
	ShaderState Shader::copy_state() {
		return m_state;
	}

	void Shader::_cleanup() {
		m_sources[0].clear();
		m_sources[1].clear();
		m_sources[2].clear();
		m_sources[3].clear();
	}

	void Shader::add_shader(const char* t_filename, shadertype t_type) {

		// Let it know what type of shader it needs to compile too. This is used when user wants to compile the shader
		switch(t_type) {

			case shadertype::vertex:
				m_state.set(ShaderState::HAS_VS);
				message(MType::DEBUG, "set ShaderState::HAS_VS");
				break;
			
			case shadertype::fragment:
				m_state.set(ShaderState::HAS_FS);
				message(MType::DEBUG, "set ShaderState::HAS_FS");
				break;
			
			case shadertype::geometry:
				m_state.set(ShaderState::HAS_GS);
				message(MType::DEBUG, "set ShaderState::HAS_GS");
				break;
			
			case shadertype::compute:
				m_state.set(ShaderState::HAS_CS);
				message(MType::DEBUG, "set ShaderState::HAS_CS");
				break;

			default:return;
		}
		
		const int type = static_cast<int>(t_type);
		_read_sources(t_filename, m_sources[type]);
		
		// And save this filename for later use...
		m_filenames[type] = strdup(t_filename); 
	}

	void Shader::add_shaders_from_memory(const char* t_vssrc, const char* t_fssrc) {
		m_sources[(int)shadertype::vertex] = t_vssrc;
		m_sources[(int)shadertype::fragment] = t_fssrc;
		m_state.set(ShaderState::HAS_VS);
		m_state.set(ShaderState::HAS_FS);
	}

	bool Shader::_compile_shader(const int t_shader, const char* t_src) {
		glShaderSource(t_shader, 1, &t_src, NULL);
		glCompileShader(t_shader);
		return _shader_ok(t_shader);
	}

	bool Shader::compile() {
	
		Timer timer;
		if(!m_raw) {
			_safe_check_vertex_source(); // Make sure there is something.
			_add_functions();
		}

		const char* const vssrc = m_sources[ (int)shadertype::vertex   ].c_str();
		const char* const fssrc = m_sources[ (int)shadertype::fragment ].c_str();
		const char* const gssrc = m_sources[ (int)shadertype::geometry ].c_str();
		const char* const cssrc = m_sources[ (int)shadertype::compute  ].c_str();
		
		// Compute shaders cant be linked with other type of shaders.
		const bool cs_ok = (m_state[ShaderState::HAS_CS] && strlen(cssrc) > 1);
		const bool vs_ok = (!cs_ok && m_state[ShaderState::HAS_VS] && strlen(vssrc) > 1);
		const bool fs_ok = (!cs_ok && m_state[ShaderState::HAS_FS] && strlen(fssrc) > 1);
		const bool gs_ok = (!cs_ok && m_state[ShaderState::HAS_GS] && strlen(gssrc) > 1);	

		if(!vs_ok && !cs_ok) {
			message(MType::ERROR, "No vertex shader was added. Please add it yourself if you are using \"raw\" shaders else visit <link to bug report page>");
			_cleanup();
			return false;
		}

		if(!fs_ok && !cs_ok) {
			message(MType::ERROR, "No fragment shader was added.");
			_cleanup();
			return false;
		}
		
		// All other shaders are optional.

		int compute_shader   = cs_ok ? glCreateShader(GL_COMPUTE_SHADER)  : -1;
		int geometry_shader  = gs_ok ? glCreateShader(GL_GEOMETRY_SHADER) : -1;
		int vertex_shader    = vs_ok ? glCreateShader(GL_VERTEX_SHADER)   : -1;
		int fragment_shader  = fs_ok ? glCreateShader(GL_FRAGMENT_SHADER) : -1;

		if(cs_ok) {
			if(!_compile_shader(compute_shader, cssrc)) {
				message(MType::ERROR, "Failed to compile [compute shader] from file \"%s\"", m_filenames[(int)shadertype::compute]);
				_cleanup();
				return false;
			}
		}
		else {
			if(!_compile_shader(vertex_shader, vssrc)) {
				message(MType::ERROR, "Failed to compile [vertex shader] from file \"%s\"", m_filenames[(int)shadertype::vertex]);
				_cleanup();
				return false;
			}

			if(!_compile_shader(fragment_shader, fssrc)) {
				message(MType::ERROR, "Failed to compile [fragment shader] from file \"%s\"", m_filenames[(int)shadertype::fragment]);
				_cleanup();
				return false;
			}

			// geometry shader
			if(gs_ok) {
				if(!_compile_shader(geometry_shader, gssrc)) {
					message(MType::ERROR, "Failed to compile [geometry shader] from file \"%s\"", m_filenames[(int)shadertype::geometry]);
					_cleanup();
					return false;
				}
			}
		}

		// Finally try to create and link the program!
		
		int program = glCreateProgram();
		if(cs_ok) {
			glAttachShader(program, compute_shader);
		}
		else {
			glAttachShader(program, vertex_shader);
			glAttachShader(program, fragment_shader);
			if(gs_ok) {
				glAttachShader(program, geometry_shader);
			}
		}

		glLinkProgram(program);
		
		// They are compiled and linked to the shader program so they can be deleted.
		if(cs_ok) {
			glDeleteShader(compute_shader);
		}
		else {
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			if(gs_ok) {
				glDeleteShader(geometry_shader);
			}
		}

		int linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(linked == 0) {
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* msg = (char*)malloc(length);
			msg[length-2] = 0; // remove '\n'
			glGetProgramInfoLog(program, length, NULL, msg);
			message(MType::ERROR, "(Shader Program Link Error): %s", msg);
			id = program;
			m_loaded = false;
			free(msg);
			_cleanup();
			return false;
		}
		id = program;
		m_loaded = true;

		message(MType::DEBUG, "Shader finished in %ims", timer.elapsed_ms());
		_cleanup();
		return true;
	}

	void Shader::set_color(const char* t_name, const Color& t_color) const {
		glUniform4f(glGetUniformLocation(id, t_name), t_color.r/255.0f, t_color.g/255.0f, t_color.b/255.0f, t_color.a/255.0f);
	}
	
	int Shader::_retrieve_location(const char* t_name) const {
		// If name is not found in saved names then ask opengl if it can find it.
		// If opengl cant find it then just dont do anything with it anymore, but if its found then add it to saved names.
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
	
	void Shader::set_vec4(const char* t_name, const Vec4& t_v4) const {
		glUniform4f(_retrieve_location(t_name), t_v4.x, t_v4.y, t_v4.z, t_v4.w);
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
		m_raw = false;
		m_state.clear();
		m_sources[0].clear();
		m_sources[1].clear();
		m_sources[2].clear();
		m_sources[3].clear();
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
		
			GLint length = 0;
			glGetShaderiv(t_id, GL_INFO_LOG_LENGTH, &length);
			
			char* msg = (char*)malloc(length);
			glGetShaderInfoLog(t_id, length, NULL, msg);
			msg[length-2] = 0; // remove '\n' from info log

			message(MType::ERROR, "---= Shader Compile Error =---\n%s", msg);

			std::string msg_str(msg, length);

			const size_t start_pos = msg_str.find(':');
			const size_t end_pos = msg_str.find('(', start_pos);
			if(start_pos == std::string::npos || end_pos == std::string::npos) {
				free(msg);
				return false;
			}

			const std::string& line_num_str = msg_str.substr(start_pos+1, end_pos-start_pos-1);

			uint32_t reach = 0;
			uint32_t line_count = 0;
			
			// make sure it is a number first
			if(line_num_str.find_first_not_of("0123456789") == std::string::npos) {
				reach = std::stoi(line_num_str)-1;
			}

			std::string& src = m_sources[(int)shadertype::compute];

			for(size_t i = 0; i < src.length(); i++) {
				if(src[i] == '\n') {
					line_count++;
					if(line_count == reach) {
						printf("\033[36m[Line %i]: \033[33m\"%s\"\033[0m\n", reach, src.substr(i+1, src.find('\n', i+1)-i-1).c_str());
						break;
					}
				}
			}
			
			free(msg);
			m_loaded = false;
			return false;
			
		}
		return true;
	}



	void Shader::_add_functions() {

		if(m_state[ShaderState::HAS_CS]) {
			m_sources[(int)shadertype::compute].insert(0,
					"#version 430 core\n"
					"#extension GL_ARB_compute_shader: enable\n"
					"#extension GL_ARB_shader_storage_buffer_object: enable\n"
					);
		}
		else {
			m_sources[(int)shadertype::fragment].insert(0,
				"#version 400 core\n"

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
				"}");
		}
	}
	
	void Shader::_safe_check_vertex_source() {
		// make sure that vertex source is not empty and it starts with '#' else update it

		m_state.set(ShaderState::HAS_VS);
		std::string& vssrc = m_sources[(int)shadertype::vertex];
		if(vssrc.size() <= 5 && vssrc.find('#') != 0) {
			vssrc.clear();
		   	vssrc = 
				"#version 400 core\n"
				"layout(location = 0) in vec3 pos;\n"
				"layout(location = 1) in vec3 normal;\n"
				"layout(location = 2) in vec2 texcoord;\n"

				"layout(location = 3) in vec4 offset_color;\n"
				"layout(location = 4) in vec3 point_pos;\n"
				"layout(location = 5) in mat4 offset;\n"
				
				"layout(location = 6) in vec4 particle_pos;"
				"layout(location = 7) in vec4 particle_prev_pos;"
				"layout(location = 8) in vec4 particle_color;"

				"layout (std140) uniform vertex_data {"
					" uniform mat4 matrix;"
					" uniform vec3 cam_pos;"
				"};"

				"struct Fragment {"
					" vec3 pos;"
					" vec3 normal;"
					" vec2 texcoord;"
					" vec3 texcoord3d;"
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
							"shape.pos = point_pos;"
							"shape.color = offset_color;"
							"fragment.pos = point_pos;"
							"fragment.normal = vec3(0.0f, 0.0f, 0.0f);"
							"gl_Position = matrix*vec4(point_pos, 1.0);"
						"break;"
	
						"case 3:" // shader_mode__particle  (TESTING)
							"shape.pos = particle_pos.xyz;"
							"shape.color = particle_color;"
							"fragment.pos = particle_pos.xyz;"
							"fragment.normal = vec3(0.0f, 0.0f, 0.0f);"
							"gl_Position = matrix*vec4(particle_pos.xyz, 1.0);"
						"break;"

						"default:break;"
					"}"
				"}";
		}
	}
}


