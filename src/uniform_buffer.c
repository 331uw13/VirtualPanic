#include <stddef.h>
#include "libs/gl3w.h"

#include "uniform_buffer.h"



VUniformBuffer VCreateUniformBuffer(uint32 size, uint32 binding_point) {
	VUniformBuffer ubo;
	glGenBuffers(1, &ubo.id);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo.id);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, ubo.id, 0, size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return ubo;
}


void VDestroyUniformBuffer(VUniformBuffer* ubo) {
	glDeleteBuffers(1, &ubo->id);
	ubo->id = 0;
	ubo->max_size = 0;
}


void VUniformBufferData(VUniformBuffer* ubo, void* data, uint32 data_size, uint32 data_offset) {
	if(ubo == NULL) { return; }
	if(data_size > ubo->max_size) { return; }

	glBindBuffer(GL_UNIFORM_BUFFER, ubo->id);
	glBufferSubData(GL_UNIFORM_BUFFER, data_offset, data_size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


