#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/gl3w.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

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
					" vec2 texcoord;\n"
				"};\n"

				"struct Material {\n"
					" vec4 color;"
					" float reflectivity;"
					//" float smoothness;"
					//" float test;"
				"};\n"
				
				"struct Light {\n"
					" vec4 pos;\n" 
					" vec4 direction;\n"
					" vec4 color;\n"
					" float radius;\n"
					" float ambience;\n"
					" float diffusion;\n"
					" float specularity;\n"
				"};\n"
				
				"layout(std140, binding = 82) uniform vpanic_fragment_data {\n"
					" Light lights[];\n"
				"};\n"

				"#define POINT_LIGHT 0\n"
				"#define DIRECTIONAL_LIGHT 1\n"

				"in vec3 camera_pos;\n"
				"in Fragment frag;\n"

				"uniform sampler2D texture0;\n"
				"uniform Material material;\n"

				"vec3 compute_light(Light light, int type) {\n"
					"vec3 item_color = material.color.xyz * light.color.xyz;\n"
					
					"vec3 v = (type == POINT_LIGHT) ? light.pos.xyz - frag.pos : light.direction.xyz;\n"
					"vec3 light_dir = normalize(v);\n"
					"vec3 norm = normalize(frag.normal);\n"
					"vec3 view_dir = normalize(camera_pos - frag.pos);\n"
					"vec3 halfway_dir = normalize(light_dir + view_dir);\n"
					//"vec3 reflect_dir = reflect(-light_dir, norm);\n"
					
					"float diff = max(dot(norm, light_dir), 0.0f);\n"
					"vec3 diffuse = light.diffusion * diff * item_color;\n"

					"float spec = pow(max(dot(halfway_dir, norm), 0.0f), max(material.reflectivity, 1.0f));\n"
					"vec3 specular = light.specularity * spec * item_color;\n"

					"vec3 ambient = light.ambience * item_color;\n"

					"if(type == POINT_LIGHT) {\n"
						"light.radius *= 0.5f;\n"
						"light.radius = max(light.radius, 0.0f);"
						"float dist = length(light.pos.xyz - frag.pos);\n"
						"float att = smoothstep(light.radius + dist, 0.0f, dist);\n"

						"diffuse *= att;\n"
						"specular *= att;\n"
						"ambient *= att;\n"
					"}\n"

					"return ambient+diffuse+specular;\n"
				"}\n";


			const int src_length = strlen(src);
			const int src_utils_length = strlen(src_utils);
		
			char* buf = NULL;
			int buf_length = src_length + src_utils_length;
			buf = malloc(buf_length+1);

			if(buf == NULL) {
				VMessage(VMSG_ERROR, "Failed to allocate memory for shader source!");
				glDeleteShader(module_id);
				module_id = 0;
			}
			else {
				memmove(buf, src_utils, src_utils_length);
				memmove(buf + src_utils_length, src, src_length);
				buf[buf_length] = '\0';
		
				glShaderSource(module_id, 1, (const char* const*)&buf, NULL);
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
	}

	return module_id;
}


void VCoreCompileDefaultVertexModule() {
	const char buf[] =
		"#version 430 core\n"
		"layout(location = 0) in vec3 i_pos;\n"
		"layout(location = 1) in vec3 i_normal;\n"
		//"layout(location = 2) in vec2 i_texcoord;\n"

		"layout (std140, binding = 83) uniform vpanic_vertex_data {\n"
			" uniform mat4 view;\n"
			" uniform mat4 proj;\n"
			" uniform vec3 cam_pos;\n"
		"};\n"

		"struct Fragment {\n"
			"vec3 pos;\n"
			"vec3 normal;\n"
			"vec2 texcoord;\n"
		"};\n"

		"out vec3 camera_pos;\n"
		"out Fragment frag;\n"
		
		"uniform mat4 model_matrix;\n"


		"void main() {\n"
			
			" vec4 pos = proj * view * model_matrix * vec4(i_pos, 1.0f);\n"
			" gl_Position = pos;\n"
			
			" frag.texcoord.x = i_pos.x+0.5f;"
			" frag.texcoord.y = i_pos.z+0.5f;"
			" frag.pos = vec3(model_matrix * vec4(i_pos, 1.0f));\n"
			//" frag.normal = -i_normal;"
			" frag.normal = mat3(transpose(inverse(model_matrix)))*i_normal;\n"
			//" frag.normal = normalize(frag.normal);"
			" camera_pos = cam_pos;\n"

		"}\n";

	vertex_module = VCoreCompileShaderModule(buf, GL_VERTEX_SHADER, VCORE_COMPILE_INTERNAL_SHADER);
	if(!VCoreIsShaderOk(vertex_module, VCORE_SHADER_MODULE)) {
		VMessage(VMSG_ERROR, "Failed to compile default vertex module! Please visit <link to bug report page>");
	}
}


uint32 VCoreLinkModules(uint32* modules, uint32 size) {
	uint32 program_id = 0;
	if(modules != NULL && size > 0) {
		program_id = glCreateProgram();
		if(program_id == 0) {
			VMessage(VMSG_ERROR, "Failed to create shader program!");
		}
		else {
			
			for(uint32 i = 0; i < size; i++) {
				glAttachShader(program_id, modules[i]);
			}

			glLinkProgram(program_id);
			
			for(uint32 i = 0; i < size; i++) {
				glDeleteShader(modules[i]);
			}
			
			if(!VCoreIsShaderOk(program_id, VCORE_SHADER_PROGRAM)) {
				VMessage(VMSG_ERROR, "Failed to link shader!");
				glDeleteProgram(program_id);
				program_id = 0;
			}
		}
	}
	return program_id;
}


uint32 VCoreLinkToInternalVertexModule(uint32 module_id) {
	uint32 m[2] = { module_id, vertex_module };
	return VCoreLinkModules(m, 2);
}


uint32 VCoreGetVertexModule() {
	return vertex_module;
}


void VCoreGenerateImage(const char* filename, int type) {
	int width = 0;
	int height = 0;
	int num_channels = 0;
	int format = GL_RGB;
	uint8* data = NULL;

	data = stbi_load(filename, &width, &height, &num_channels, 0);
	if(data == NULL) {
		VMessage(VMSG_ERROR, "Failed to load texture from file \"%s\"", filename);
		VMessage(VMSG_ERROR, "Reason: \"%s\"", stbi_failure_reason());
		return;
	}

	switch(num_channels) {
		case 1: format = GL_RED;   break;
		case 2: format = GL_RG;    break;
		case 3: format = GL_RGB;   break;
		case 4: format = GL_RGBA;  break;
		default: 
			VMessage(VMSG_WARNING, "Failed to pickup format for \"%s\". Using GL_RGB.", filename);
			break;
	}

	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(type);
	stbi_image_free(data);
	
	VMessage(VMSG_OK, "Loaded texture \"%s\"", filename);
}


