#ifndef VIRTUAL_PANIC__RENDER_H
#define VIRTUAL_PANIC__RENDER_H

#include "renderable.h"


void VCreateNewShape(VRenderData* rdata, float* points, uint32 size);
void VCreateNewBox(VRenderData* rdata);
void VCreateNewPlane(VRenderData* rdata);

void VDestroyRenderData(VRenderData* rdata);

void VSetWireframeEnabled(uint8 b);
void VRender(VRenderData* rdata);

#endif
