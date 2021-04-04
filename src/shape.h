#ifndef VIRTUAL_PANIC__SHAPE_H
#define VIRTUAL_PANIC__SHAPE_H

#include "types.h"
#include "matrix.h"


typedef struct {
	uint32 id;
	uint32 shader;
	VMatrix matrix;
	//VDrawData draw_data; // normals, normal_count, vao, vbo
} VShape;




#endif
