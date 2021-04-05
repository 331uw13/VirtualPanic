#ifndef VIRTUAL_PANIC__RENDERABLE_H
#define VIRTUAL_PANIC__RENDERABLE_H

#include "matrix.h"
#include "types.h"


typedef struct {
	uint32 vao;
	uint32 vbo;

} VRenderData;

typedef struct {
	RGBA color;
	VMatrix mtrx;
	VRenderData rdata;
} VBox;

#endif
