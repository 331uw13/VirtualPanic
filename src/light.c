#include <stddef.h>

#include "light.h"
#include "uniform_buffer.h"

static VUniformBuffer light_ubo;


void VCreateLightUniformBuffer() {
	light_ubo = VCreateUniformBuffer(sizeof(VLight)*2, 82);
}


void VMaxLights(uint32 count) {
	// ...
}


void VUpdateLights(VLight* lights, uint32 size) {

	// TODO: map buffer.. its faster.
	
	//VUniformBufferData(&light_ubo, lights, sizeof *lights * size, 0);
	
	uint32 off = 0;
	for(uint32 i = 0; i < size; i++) {

		VLight* l = &lights[i];

		VUniformBufferData(&light_ubo, &l->position, sizeof(Vector4), off);
		VUniformBufferData(&light_ubo, &l->color,    sizeof(Vector4), off + sizeof(Vector4));
		
		VUniformBufferData(&light_ubo, &l->radius,      sizeof(float),   off + sizeof(Vector4)*2);
		VUniformBufferData(&light_ubo, &l->ambience,    sizeof(float),   off + sizeof(Vector4)*2 + sizeof(float));
		VUniformBufferData(&light_ubo, &l->diffusion,   sizeof(float),   off + sizeof(Vector4)*2 + sizeof(float)*2);
		VUniformBufferData(&light_ubo, &l->specularity, sizeof(float),   off + sizeof(Vector4)*2 + sizeof(float)*3);

		/*
		VUniformBufferData(&light_ubo, &l->position.x, sizeof(float), off);
		VUniformBufferData(&light_ubo, &l->position.y, sizeof(float), off + sizeof(float));
		VUniformBufferData(&light_ubo, &l->position.z, sizeof(float), off + sizeof(float) * 2);
		*/
		// < padding >
	
		/*
		VUniformBufferData(&light_ubo, &l->color.x, sizeof(float), off + sizeof(float) * 4);
		VUniformBufferData(&light_ubo, &l->color.y, sizeof(float), off + sizeof(float) * 5);
		VUniformBufferData(&light_ubo, &l->color.z, sizeof(float), off + sizeof(float) * 6);
		VUniformBufferData(&light_ubo, &l->color.w, sizeof(float), off + sizeof(float) * 7);
		*/
		off += sizeof *l;
	}
	


}

