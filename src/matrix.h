#ifndef VIRTUAL_PANIC__MATRIX_H
#define VIRTUAL_PANIC__MATRIX_H

#include "types.h"


typedef struct {
	float data[4][4];
} VMatrix;


void VNullMatrix(VMatrix* matrix);

void VMatrixScale(VMatrix* dest, float x, float y, float z);
void VMatrixTranslate(VMatrix* dest, float x, float y, float z);
void VMatrixMerge(VMatrix* dest, VMatrix* a, VMatrix* b);

void VMatrixRotateX(VMatrix* dest, float angle);
void VMatrixRotateY(VMatrix* dest, float angle);
void VMatrixRotateZ(VMatrix* dest, float angle);

/*
void VMatrixCopyRotation(VMatrix* dest, VMatrix* src);

Vector3 VGetMatrixTranslation(VMatrix* m);
Vector3 VGetMatrixRotation(VMatrix* m);
*/
void VComputeProjectionMatrix(VMatrix* dest, float fov, float aspect_ratio, float z_near, float z_far);
void VComputeViewMatrix(VMatrix* dest, Vector3 position, Vector3 direction);


#endif
