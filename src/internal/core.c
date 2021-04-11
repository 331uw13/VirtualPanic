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
	
	if(flag == VCORE_COMPILE_USER_SHADER) {
		const char src_utils[] = 
			"#version 430 core\n"
		
			"struct Fragment {"
				"vec3 pos;"
				"vec3 normal;"
			"};"

			"in vec3 camera_pos;"
			"in Fragment frag;"

			// TODO: Light system!
			"\n"
			"#define LIGHT_AMBIENT   1.0f\n"
			"#define LIGHT_DIFFUSE   1.0f\n"
			"#define LIGHT_SPECULAR  1.0f\n"
			"#define LIGHT_SHINE     64.f\n"
			"vec3 light_pos = vec3(0.0f, 3.0f, 0.0f);"
			"vec4 light_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
			"float light_radius = 15.0f;"

			"vec4 shape_color = vec4(0.05f, 1.0f, 0.05f, 1.0f);"


			"vec3 compute_light() {"
				" vec3 item_color = shape_color.xyz * light_color.xyz;"
				
				" vec3 norm = normalize(frag.normal);"
				" vec3 light_dir = normalize(light_pos - frag.pos);"
				" vec3 view_dir = normalize(camera_pos - frag.pos);"
				" vec3 reflect_dir = reflect(-light_dir, norm);"
				
				// Diffuse
				"float diff = max(dot(norm, light_dir), 0.0f);"
				"vec3 diffuse = LIGHT_DIFFUSE * diff * item_color;"

				// Specular
				"float spec = pow(max(dot(normalize(light_dir + view_dir), norm), 0.0f), LIGHT_SHINE);"
				"vec3 specular = LIGHT_SPECULAR * spec * item_color;"

				// Ambient
				" vec3 ambient = LIGHT_AMBIENT * item_color;"


				"light_radius *= 0.5f;"
				"float dist = length(light_pos - frag.pos);"
				"float att = smoothstep(light_radius + dist, 0.0f, dist);"

				"diffuse *= att;"
				"specular *= att;"
				"ambient *= att;"

				"return ambient+diffuse+specular;"
			"}";

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
			glShaderSource(module_id, 1, &buf, NULL);
			free(buf);
		}
	}
	else {
		glShaderSource(module_id, 1, &src, NULL);
	}

	glCompileShader(module_id);

	if(!VCoreIsShaderOk(module_id, VCORE_SHADER_MODULE)) {
		VMessage(VMSG_ERROR, "Failed to compile shader module!");
		glDeleteShader(module_id);
		module_id = 0;
	}
	
	return module_id;
}


void VCoreCompileDefaultVertexModule() {
	const char buf[] =
		"#version 430 core\n"
		"layout(location = 0) in vec3 i_pos;\n"
		"layout(location = 1) in vec3 i_normal;\n"
		"layout (std140, binding = 0) uniform vpanic__vertex_data {\n"
			" uniform mat4 view;"
			" uniform mat4 proj;"
		"};"
			
		"struct Fragment {"
			"vec3 pos;"
			"vec3 normal;"
		"};"

		"out vec3 camera_pos;"
		"out Fragment frag;"
		
		"uniform mat4 model_matrix;"


		"void main() {"
			" vec4 pos = proj * view * model_matrix * vec4(i_pos, 1.0f);"
			" gl_Position = pos;"

			" frag.pos = pos.xyz;"
			" frag.normal = pos.xyz;"
			" camera_pos = view[3].xyz;"

		"}";

	vertex_module = VCoreCompileShaderModule(buf, GL_VERTEX_SHADER, VCORE_COMPILE_INTERNAL_SHADER);
	if(!VCoreIsShaderOk(vertex_module, VCORE_SHADER_MODULE)) {
		VMessage(VMSG_ERROR, "Failed to compile default vertex module! Please visit <link to bug report page>");
	}
}


uint32 VCoreLinkShaderModule(uint32 module_id) {
	uint32 program_id = glCreateProgram();	

	glAttachShader(program_id, vertex_module);
	glAttachShader(program_id, module_id);
	glLinkProgram(program_id);

	glDeleteShader(module_id);
	if(!VCoreIsShaderOk(program_id, VCORE_SHADER_PROGRAM)) {
		VMessage(VMSG_ERROR, "Failed to link shader!");
		glDeleteProgram(program_id);
		program_id = 0;
	}

	return program_id;
}


uint32 VCoreGetVertexModule() {
	return vertex_module;
}


