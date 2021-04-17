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
		VShaderAddUniformBlockBinding(shader, "vpanic_vertex_data", 83);
		VShaderAddUniformBlockBinding(shader, "vpanic_fragment_data", 82);

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
	const uint32 i = glGetUniformBlockIndex(shader, block_name);
	if(i != GL_INVALID_INDEX) {
		glUniformBlockBinding(shader, i, binding_point);
	}
	else {
		VMessage(VMSG_ERROR, "\"%s\" is not an active uniform block!", block_name);
	}
}


// TODO: optimize this!
// currently its getting uniform location for everytime setting it..

void VShaderSetMatrix(VShader shader, const char* name, VMatrix* m) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &m->data[0][0]);
}


void VShaderSetVector3(VShader shader, const char* name, Vector3* v) {
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, name), v->x, v->y, v->z);
}


void VShaderSetVector4(VShader shader, const char* name, Vector4* v) {
	glUseProgram(shader);
	glUniform4f(glGetUniformLocation(shader, name), v->x, v->y, v->z, v->w);
}


void VShaderSetFloat(VShader shader, const char* name, float f) {
	glUseProgram(shader);
	glUniform1f(glGetUniformLocation(shader, name), f);
}


void VShaderSetInt(VShader shader, const char* name, int i) {
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, name), i);
}





