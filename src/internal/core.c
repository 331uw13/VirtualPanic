#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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
	glShaderSource(module_id, 1, &src, NULL);
	glCompileShader(module_id);

	if(!VCoreIsShaderOk(module_id, VCORE_SHADER_MODULE)) {
		VMessage(VMSG_ERROR, "Failed to compile shader module!");
		glDeleteShader(module_id);
		module_id = 0;
	}
	
	return module_id;
}

void VCoreCompileDefaultVertexModule() {
	const char buf[] = {
		"#version 430 core\n"
		"layout(location = 0) in vec3 i_pos;\n"
		"layout(location = 1) in vec3 i_normal;\n"

		"uniform mat4 proj;"
		"uniform mat4 view;"
		"uniform mat4 model;"

		"void main() {"
			" gl_Position = proj*view*model*vec4(i_pos, 1.0f);"
		"}"};

	vertex_module = VCoreCompileShaderModule(buf, GL_VERTEX_SHADER, VCORE_COMPILE_INTERNAL_SHADER);
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
	else {
		// ...
	}

	return program_id;
}



