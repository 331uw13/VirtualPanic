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
	const uint32 module_id = VCoreCompileShaderModule(src, GL_FRAGMENT_SHADER, VCORE_COMPILE_USER_SHADER);
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


void VDestroyShader(VShader* id) {
	if(id == NULL || *id == 0) { return; }
	VMessage(VMSG_DEBUG, __FUNCTION__);
	glDeleteProgram(*id);
	*id = 0;
}


void VShaderAddUniformBlockBinding(VShader shader, const char* block_name, uint32 binding_point) {
	VMessage(VMSG_DEBUG, __FUNCTION__);
	const uint32 i = glGetUniformBlockIndex(shader, block_name);
	if(i != GL_INVALID_INDEX) {
		glUniformBlockBinding(shader, i, binding_point);
	}
	else {
		VMessage(VMSG_ERROR, "\"%s\" is not an active uniform block!", block_name);
	}
}


void VShaderSetMatrix(VShader shader, uint32 uniform_index, VMatrix* m) {
	glUseProgram(shader);
	glUniformMatrix4fv(uniform_index, 1, GL_FALSE, &m->data[0][0]);
}


void VShaderSetColor(VShader shader, uint32 uniform_index, VColor* c) {
	glUseProgram(shader);
	glUniform4f(uniform_index, (float)c->r/255.f, (float)c->g/255.f, (float)c->b/255.f, (float)c->a/255.f);
}


void VShaderSetVector3(VShader shader, uint32 uniform_index, Vector3* v) {
	glUseProgram(shader);
	glUniform3f(uniform_index, v->x, v->y, v->z);
}


void VShaderSetFloat(VShader shader, uint32 uniform_index, float f) {
	glUseProgram(shader);
	glUniform1f(uniform_index, f);
}


void VShaderSetInt(VShader shader, uint32 uniform_index, int i) {
	glUseProgram(shader);
	glUniform1i(uniform_index, i);
}





