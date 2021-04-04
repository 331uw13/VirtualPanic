#ifndef VIRTUAL_PANIC__UTILS_H
#define VIRTUAL_PANIC__UTILS_H

#include <math.h>
#include "types.h"


float VRadians(float a);
float VLength(Vector3 v);
float VDot(Vector3 v0, Vector3 v1);
Vector3 VNormalized(Vector3 v);
Vector3 VCross(Vector3 v0, Vector3 v1);


#endif
