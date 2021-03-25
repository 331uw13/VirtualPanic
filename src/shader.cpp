#include <cstring>
#include <numeric>
#include "libs/gl3w.h"

#include "shader.hpp"
#include "messages.hpp"
#include "timer.hpp"
#include "file.hpp"


namespace vpanic {

	Shader::~Shader() {
		unload();
	}
	
	void Shader::_clear_sources() {
	}
	
	bool Shader::add_src(const char* t_filename, const int t_type) {
		bool error_occurred = false;
		ShaderComponent& sc = m_components[t_type];
		
		if(t_type >= INVALID_SHADER || t_type < 0) {
			error_occurred = true;
		}
		else {
			Timer timer;
			_add_functions(sc.src, t_type);

			// included from "file.hpp"
			if(read_file(t_filename, &sc.src)) {
				sc.compile(t_type);
				if(_succeeded(sc.id, SHADER)) {
					m_type_bits |= t_type;
					message(MType::OK, "Compiled shader component \"%s\" (took %ims)", t_filename, timer.elapsed_ms());
				}
				else {
					sc.type = INVALID_SHADER;
					message(MType::ERROR, "Failed to compile shader component \"%s\"", t_filename);
					error_occurred = true;
				}
			}	
			//printf("\033[32m  \"%s\"\n\033[90m%s\033[0m\n", t_filename, sc.src.c_str());
		}
		
		return !error_occurred && sc.type != INVALID_SHADER;
	}
	
	bool Shader::add_src_from_memory(const std::string& t_src, const int t_type) {
		bool error_occurred = false;
		ShaderComponent& sc = m_components[t_type];
	
		if(t_type >= INVALID_SHADER || t_type < 0) {
			error_occurred = true;
		}
		else {
			Timer timer;
			sc.src = std::move(t_src);
			_add_functions(sc.src, t_type);
			sc.compile(t_type);
			if(_succeeded(sc.id, SHADER)) {
				m_type_bits |= t_type;
				message(MType::OK, "Compiled shader component (%i) (took %ims)", t_type, timer.elapsed_ms());
			}
			else {
				sc.type = INVALID_SHADER;
				message(MType::ERROR, "Failed to compile shader component (%i)", t_type);
				error_occurred = true;
			}
		}

		return !error_occurred && sc.type != INVALID_SHADER;
	}

	bool Shader::_succeeded(const int t_id, const int t_type) {	
		char* msg = nullptr;
		bool error_occurred = (t_type != SHADER && t_type != PROGRAM);
		GLsizei max_length = 0;
		GLsizei log_length = 0;
		GLint p = 0;

		if(!error_occurred) {
			if(t_type == SHADER) {
				glGetShaderiv(t_id, GL_COMPILE_STATUS, &p);
				glGetShaderiv(t_id, GL_INFO_LOG_LENGTH, &max_length);
			}
			else if(t_type == PROGRAM) {
				glGetProgramiv(t_id, GL_LINK_STATUS, &p);
				glGetProgramiv(t_id, GL_INFO_LOG_LENGTH, &max_length);
			}

			if(max_length > 1) {
				msg = (char*)malloc(max_length);
				if(t_type == SHADER) {
					glGetShaderInfoLog(t_id, max_length, &log_length, msg);
				}
				else if(t_type == PROGRAM) {
					glGetProgramInfoLog(t_id, max_length, &log_length, msg);
				}
				msg[max_length-2] = 0; // remove '\n'
				if(p != GL_TRUE) {
					error_occurred = true;
					message(MType::SHADER_ERROR, "%s", msg);
				}
				else {
					message(MType::SHADER_WARNING, "%s", msg);
				}
				free(msg);
			}
		}

		return !error_occurred;
	}

	bool Shader::compile() {

		// Compute shaders cant be linked with any other type of shader.
		
		bool error_occurred = false;
		ShaderComponent& vertex = m_components[VERTEX_SHADER];
		const ShaderComponent& fragment = m_components[FRAGMENT_SHADER];
		const ShaderComponent& geometry = m_components[GEOMETRY_SHADER];
		const ShaderComponent& compute = m_components[COMPUTE_SHADER];
		Timer timer;

		id = glCreateProgram();

		if(compute.type != INVALID_SHADER && (m_type_bits & COMPUTE_SHADER)) {	
			glAttachShader(id, compute.id);
		}
		else if(fragment.type != INVALID_SHADER && (m_type_bits & FRAGMENT_SHADER)) {
			const bool has_vs = vertex.type != INVALID_SHADER && (m_type_bits & VERTEX_SHADER);
			const bool has_gs = geometry.type != INVALID_SHADER && (m_type_bits & GEOMETRY_SHADER);

			if(!has_vs && !add_src_from_memory(vertex.src, VERTEX_SHADER)) {
				// thats bad... ;-;
				message(MType::ERROR, "Oops! Failed to compile vertex shader from memory. Please visit <link to bug report page>");
				error_occurred = true;
			}
			
			if(!error_occurred) {
				glAttachShader(id, vertex.id);
				glAttachShader(id, fragment.id);
				if(has_gs) {
					glAttachShader(id, geometry.id);
				}
			}
		}

		glLinkProgram(id);
		if(!_succeeded(id, PROGRAM)) {
			error_occurred = true;
			glDeleteProgram(id);
			id = 0;
		}
		else {
			message(MType::OK, "Compiled shader program (took %ims)", timer.elapsed_ms());
		}

		_clear_sources();
		return !error_occurred;
	}


	void Shader::set_color(const char* t_name, const Color& t_color) const {
		glUniform4f(glGetUniformLocation(id, t_name), t_color.r/255.0f, t_color.g/255.0f, t_color.b/255.0f, t_color.a/255.0f);
	}
	
	int Shader::_retrieve_location(const char* t_name) const {
		// If name is not found in saved names then ask opengl if it can find it.
		// If opengl cant find it then just dont do anything with it anymore, but if its found then add it to saved names.
		int location = -1;
		if(m_saved.find(t_name) == m_saved.end()) {
			const int l = glGetUniformLocation(id, t_name);
			if(l >= 0) {
				m_saved[t_name] = l;
			}
			location = l;
		}
		else {
			location = m_saved[t_name];
		}
		
		return location;
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
	
	void Shader::disable() const {
		glUseProgram(0);
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
		m_type_bits = 0;
		_clear_sources();
	}

	void Shader::_add_functions(std::string& t_src, const int t_type) {

		if(t_type == COMPUTE_SHADER) {
			t_src.insert(0,
					"#version 430 core\n"
					"#extension GL_ARB_compute_shader: enable\n"
					"#extension GL_ARB_shader_storage_buffer_object: enable\n"
					
					"uniform int max_particles;"
					"uniform float dt;"
					"uniform vec4 origin;"
					"uniform bool mouse_left_down;"
					"uniform bool mouse_right_down;"

					"struct Particle {"
						" vec4 attr0;"
						" vec4 attr1;"
						" vec4 attr2;"
						" vec3 attr3;"
						" float life;"
					"};"

					"layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;"
					"layout (std430, binding = 0) buffer particle_buffer {"
						" Particle particles[];"
					"};"

					"uint rng_state = 0;"
					"uint rand_xorshift() {"
						// Xorshift algorithm from George Marsaglia's paper
						" rng_state ^= (rng_state << 13);"
						" rng_state ^= (rng_state >> 17);"
						" rng_state ^= (rng_state << 5);"
						" return rng_state;"
					"}"
					"float rand() {"
						" return float(rand_xorshift())*(1.0f/4294967296.0f);"
					"}"
					"uint get_particle_id() {"
						" const uint x = gl_GlobalInvocationID.x;"
						//" if(x >= max_particles) { return 0; }"
						" rng_state = x;"
						" return x;"
					"}");

		}
		else if(t_type == FRAGMENT_SHADER) {
			t_src.insert(0,
				"#version 430 core\n"

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
				"in Fragment fragment;"
				"in VPanicShape shape;"
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
					" float att = d*smoothstep(radius/0.8f+d, 0.0, d);"
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
		else if(t_type == VERTEX_SHADER) {
			// make sure that vertex source is not empty and it starts with '#' else update it
			std::string& vssrc = m_components[VERTEX_SHADER].src;
			if(vssrc.size() <= 5 && vssrc.find('#') != 0) {
				vssrc.clear();
				vssrc = 
					"#version 430 core\n"
					"layout(location = 0) in vec3 pos;\n"
					"layout(location = 1) in vec3 normal;\n"
					"layout(location = 2) in vec2 texcoord;\n"

					// TODO: rewrite this... whole thing!

					"layout(location = 3) in vec4 offset_color;\n"
					"layout(location = 4) in vec3 point_pos;\n"
					"layout(location = 5) in mat4 offset;\n"
					"layout(location = 9) in vec4 particle_pos;\n"
					//"layout(location = 9) in vec4 particle_velocity;\n"
					"layout(location = 10) in vec4 particle_color;\n"

					
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
								"fragment.pos = shape.pos;"
								"fragment.normal = vec3(0.0f, 0.0f, 0.0f);"
								"gl_Position = matrix*vec4(shape.pos, 1.0);"
							"break;"

							"default:break;"
						"}"
					"}";
			}
		}
	}
}


