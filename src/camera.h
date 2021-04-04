#ifndef VIRTUAL_PANIC__CAMERA_H
#define VIRTUAL_PANIC__CAMERA_H

#include "types.h"
#include "matrix.h"


typedef struct {
	Vector3 position;
	Vector3 direction;
	VMatrix matrix;
	float sensetivity;
	float yaw;
	float pitch;
	uint8 noclip;
} VCamera;

#endif
