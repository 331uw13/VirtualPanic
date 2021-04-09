#ifndef VIRTUAL_PANIC__RENDER_H
#define VIRTUAL_PANIC__RENDER_H

#include "renderable.h"


VRenderData* VCreateEmptyRenderData();
VRenderData* VGetRenderData(uint32 id);
uint32 VGetRenderDataCount();

void VDestroyRenderData(VRenderData* box);
void VDestroyAllRenderData();

VRenderData* VCreateNewBox(float initial_x, float initial_y, float initial_z);
VRenderData* VCreateNewPlane(float initial_x, float initial_y, float initial_z);
VRenderData* VCreateNewShape(float* points, uint32 size);


void VRender(VRenderData* rdata);


#endif
