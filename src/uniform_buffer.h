#ifndef VIRTUAL_PANIC__UNIFORM_BUFFER_H
#define VIRTUAL_PANIC__UNIFORM_BUFFER_H

#include "types.h"


typedef struct {
	uint32 id;
	uint32 max_size;
} VUniformBuffer;


VUniformBuffer VCreateUniformBuffer(uint32 size, uint32 binding_point);
void VDestroyUniformBuffer(VUniformBuffer* ubo);
void VUniformBufferData(VUniformBuffer* ubo, void* data, uint32 data_size, uint32 data_offset);



#endif
