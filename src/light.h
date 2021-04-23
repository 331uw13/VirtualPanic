#ifndef VIRTUAL_PANIC__LIGHT_H
#define VIRTUAL_PANIC__LIGHT_H

#include "types.h"

typedef struct {
	Vector4 position;
	Vector4 direction;
	Vector4 color;
	float radius;
	float ambience;
	float diffusion;
	float specularity;
} VLight;


void VCreateLightUniformBuffer();
void VMaxLights(uint32 count);
void VUpdateLights(VLight* lights, uint32 index, uint32 size);


#endif
