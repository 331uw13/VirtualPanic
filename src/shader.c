#include <stddef.h>
#include "libs/gl3w.h"

#include "internal/core.h"
#include "shader.h"
#include "messages.h"

static VShader first_created_shader = 0;


VShader VCreateShader(const char* src) {
	if(src == NULL) { return 0; }
	VMessage(VMSG_DEBUG, __FUNCTION__);
	uint32 shader = 0;
	uint32 module_id = VCoreCompileShaderModule(src, GL_FRAGMENT_SHADER, VCORE_COMPILE_USER_SHADER);
	if(module_id != 0) {
		shader = VCoreLinkShaderModule(module_id);
		VShaderAddUniformBlockBinding(shader, "vpanic__vertex_data", 83);
		
		if(first_created_shader == 0) {
			first_created_shader = shader;
		}
	}

	return shader;
}


VShader VGetFirstCreatedShader() {
	return first_created_shader;
}


void VDestroyShader(uint32* id) {
	if(id == NULL) { return; }
	VMessage(VMSG_DEBUG, __FUNCTION__);
	glDeleteProgram(*id);
	*id = 0;
}


void VShaderAddUniformBlockBinding(uint32 shader_id, const char* block_name, uint32 binding_point) {
	VMessage(VMSG_DEBUG, __FUNCTION__);
	const uint32 i = glGetUniformBlockIndex(shader_id, block_name);
	glUniformBlockBinding(shader_id, i, binding_point);
}


