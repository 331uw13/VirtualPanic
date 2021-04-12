#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/gl3w.h"

#include "core.h"
#include "../messages.h"

#define VCORE_SHADER_MODULE 1
#define VCORE_SHADER_PROGRAM 2

static uint32 vertex_module = 0;
static uint8 VCoreIsShaderOk(uint32 id, uint8 flag);



uint8 VCoreIsShaderOk(uint32 id, uint8 flag) {
	uint8 result = 1;

	GLsizei max_length = 0;
	GLsizei log_length = 0;
	GLint p = 0;
	char* msg = NULL;
	
	if(flag == VCORE_SHADER_MODULE) {
		glGetShaderiv(id, GL_COMPILE_STATUS, &p);
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);
	}
	else if(flag == VCORE_SHADER_PROGRAM) {
		glGetProgramiv(id, GL_LINK_STATUS, &p);
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);
	}
	
	if(max_length > 1) {
		result = 0;
		msg = (char*)malloc(sizeof(char)*max_length);

		if(msg != NULL) {
			if(flag == VCORE_SHADER_MODULE) {
				glGetShaderInfoLog(id, max_length, &log_length, msg);
			}
			else if(flag == VCORE_SHADER_PROGRAM) {
				glGetProgramInfoLog(id, max_length, &log_length, msg);
			}
			
			VMessage(p ? VMSG_WARNING : VMSG_ERROR, "%s", msg);
			free(msg);
		}
	}
	
	return result;
}


uint32 VCoreCompileShaderModule(const char* src, uint32 type, uint8 flag) {
	uint32 module_id = glCreateShader(type);
	if(module_id == 0) {
		VMessage(VMSG_ERROR, "Failed to create shader module!");
	}
	else {
		if(flag == VCORE_COMPILE_USER_SHADER) {
			const char src_utils[] = 
				"#version 430 core\n"
			
				"struct Fragment {\n"
					" vec3 pos;\n"
					" vec3 normal;\n"
				"};\n"
				
				"struct Light {\n"
					" vec3 pos;\n"
					" float radius;\n"
					" float ambience;\n"
					" float diffusion;\n"
					" float specularity;\n"
				"};\n"
				
				"layout(std140, binding = 82) uniform vpanic__fragment_data {\n"
					" Light lights[];\n"
				"};\n"


				"in vec3 camera_pos;\n"
				"in Fragment frag;\n"

				// TODO: Light system!
				"\n"
				"#define LIGHT_AMBIENT   0.65f\n"
				"#define LIGHT_DIFFUSE   0.2f\n"
				"#define LIGHT_SPECULAR  0.3f\n"
				"#define LIGHT_SHINE     64.f\n"
				"vec3 light_pos = vec3(0.0f, 2.0f, 0.0f);\n"
				"vec4 light_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
				"float light_radius = 15.0f;\n"

				"vec4 shape_color = vec4(0.05f, 1.0f, 0.05f, 1.0f);\n"


				"vec3 compute_light() {\n"
					" vec3 item_color = shape_color.xyz * light_color.xyz;\n"
					
					//" light_pos = camera_pos;" // DELETE THIS

					" vec3 norm = normalize(frag.normal);\n"
					" vec3 light_dir = normalize(light_pos - frag.pos);\n"
					" vec3 view_dir = normalize(camera_pos - frag.pos);\n"
					" vec3 halfway_dir = normalize(light_dir + view_dir);\n"
					//" vec3 reflect_dir = reflect(-light_dir, norm);"
					
					// Diffuse
					"float diff = max(dot(norm, light_dir), 0.0f);\n"
					"vec3 diffuse = LIGHT_DIFFUSE * diff * item_color;\n"

					// Specular
					"float spec = pow(max(dot(halfway_dir, norm), 0.0f), LIGHT_SHINE);\n"
					"vec3 specular = LIGHT_SPECULAR * spec * item_color;\n"

					// Ambient
					" vec3 ambient = LIGHT_AMBIENT * item_color;\n"

					"light_radius *= 0.5f;\n"
					"float dist = length(light_pos - frag.pos);\n"
					"float att = smoothstep(light_radius + dist, 0.0f, dist);\n"

					"diffuse *= att;\n"
					"specular *= att;\n"
					"ambient *= att;\n"

					"return ambient+diffuse+specular;\n"
				"}\n";

			const int src_length = strlen(src);
			const int src_utils_length = strlen(src_utils);
			
			char* buf = NULL;
			int buf_length = src_length + src_utils_length;
			buf = malloc(buf_length);

			if(buf == NULL) {
				VMessage(VMSG_ERROR, "Failed to allocate memory for shader source!");
				glDeleteShader(module_id);
				module_id = 0;
			}
			else {
				memmove(buf, src_utils, src_utils_length);
				memmove(buf + src_utils_length, src, src_length);
				buf[buf_length] = '\0';
			
				VMessage(VMSG_DEBUG, "glShaderSource(%p) | module_id: %i (VCORE_COMPILE_USER_SHADER)", buf, module_id);
				glShaderSource(module_id, 1, (const char* const*)&buf, NULL);
				free(buf);
			}
		}
		else {
			VMessage(VMSG_DEBUG, "glShaderSource(%p) | module_id: %i (VCORE_COMPILE_INTERNAL_SHADER)", src, module_id);
			glShaderSource(module_id, 1, &src, NULL);
		}

		glCompileShader(module_id);
		
		if(!VCoreIsShaderOk(module_id, VCORE_SHADER_MODULE)) {
			VMessage(VMSG_ERROR, "Failed to compile shader module!");
			glDeleteShader(module_id);
			module_id = 0;
		}
	}

	return module_id;
}


void VCoreCompileDefaultVertexModule() {
	const char buf[] =
		"#version 430 core\n"
		"layout(location = 0) in vec3 i_pos;\n"
		"layout(location = 1) in vec3 i_normal;\n"

		"layout (std140, binding = 83) uniform vpanic__vertex_data {\n"
			" uniform mat4 view;\n"
			" uniform mat4 proj;\n"
			" uniform vec3 cam_pos;\n"
		"};\n"
			
		"struct Fragment {\n"
			"vec3 pos;\n"
			"vec3 normal;\n"
		"};\n"

		"out vec3 camera_pos;\n"
		"out Fragment frag;\n"
		
		"uniform mat4 model_matrix;\n"


		"void main() {\n"
			
			" vec4 pos = proj * view * model_matrix * vec4(i_pos, 1.0f);\n"
			" gl_Position = pos;\n"

			" frag.pos = vec3(model_matrix * vec4(i_pos, 1.0f));\n"
			" frag.normal = i_normal;\n"
			" camera_pos = cam_pos;\n"

		"}\n";

	vertex_module = VCoreCompileShaderModule(buf, GL_VERTEX_SHADER, VCORE_COMPILE_INTERNAL_SHADER);
	if(!VCoreIsShaderOk(vertex_module, VCORE_SHADER_MODULE)) {
		VMessage(VMSG_ERROR, "Failed to compile default vertex module! Please visit <link to bug report page>");
	}
}


uint32 VCoreLinkShaderModule(uint32 module_id) {
	uint32 program_id = glCreateProgram();	
	if(program_id == 0) {
		VMessage(VMSG_ERROR, "Failed to create shader program!");
	}
	else {
		glAttachShader(program_id, vertex_module);
		glAttachShader(program_id, module_id);
		glLinkProgram(program_id);

		glDeleteShader(module_id);
		if(!VCoreIsShaderOk(program_id, VCORE_SHADER_PROGRAM)) {
			VMessage(VMSG_ERROR, "Failed to link shader!");
			glDeleteProgram(program_id);
			program_id = 0;
		}
	}
	return program_id;
}


uint32 VCoreGetVertexModule() {
	return vertex_module;
}


