#ifndef VIRTUAL_PANIC__RENDERABLE_H
#define VIRTUAL_PANIC__RENDERABLE_H

#include "matrix.h"
#include "types.h"
#include "shader.h"
#include "texture.h"
#include "material.h"


typedef struct {
	uint32 vao;
	uint32 vbo;
	uint32 size;
	VShader shader;
	VTexture texture;
	VMatrix matrix;
	VMaterial material;
} VRenderData;



/*

// Not needed yet and needs a bit more thinking lol.

typedef struct {
	VColor color;
	Vector3 point_a;
	Vector3 point_b;
} VLine;


typedef struct {
	VRenderData rdata;
	VColor color;
	VMatrix matrix;
	float* height_map;
	uint32 size;
	uint8 wireframe;
} VTerrain;
*/


#endif
