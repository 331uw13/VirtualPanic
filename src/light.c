#include <stddef.h>
#include "libs/gl3w.h"

#include "light.h"
#include "uniform_buffer.h"


static VUniformBuffer light_ubo;


void VCreateLightUniformBuffer() {
	light_ubo = VCreateUniformBuffer(sizeof(VLight)*3, 82);
}


void VMaxLights(uint32 count) {
	if(light_ubo.id != 0) {
		glDeleteBuffers(1, &light_ubo.id);
	}
	light_ubo = VCreateUniformBuffer(sizeof(VLight)*count, 82);
}


void VUpdateLights(VLight* lights, uint32 index, uint32 size) {
	VUniformBufferData(&light_ubo, lights+index, sizeof *lights * size, sizeof *lights * index);
}

