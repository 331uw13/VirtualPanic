#ifndef VIRTUAL_PANIC__MATRIX_H
#define VIRTUAL_PANIC__MATRIX_H

#include "types.h"


typedef struct {
	float data[4][4];
} VMatrix;


void VNullMatrix(VMatrix* matrix);

// TODO: rename m to dest
void VMatrixTranslate(VMatrix* dest, float x, float y, float z);
void VMatrixScale(VMatrix* m, float x, float y, float z);
//void VMatrixRotateX(VMatrix* m, float angle);
//void VMatrixRotateY(VMatrix* m, float angle);
//void VMatrixRotateZ(VMatrix* m, float angle);

void VMatrixCopyRotation(VMatrix* dest, VMatrix* src);

void VCreateProjectionMatrix(VMatrix* dest, float fov, float aspect_ratio, float z_near, float z_far);
void VComputeViewMatrix(VMatrix* dest, Vector3 position, Vector3 direction);


#endif
