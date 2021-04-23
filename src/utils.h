#ifndef VIRTUAL_PANIC__UTILS_H
#define VIRTUAL_PANIC__UTILS_H

#include <math.h>
#include "types.h"
#include "renderable.h"

#define VFAST_RAND_MAX 0x7FFF


float VRadians(float a);
float VLength(Vector3 v);
float VDot(Vector3 v0, Vector3 v1);
Vector3 VNormalized(Vector3 v);
Vector3 VCross(Vector3 v0, Vector3 v1);
float VDistance(Vector3 v0, Vector3 v1);
float VLerp(float start, float end, float t);
void VSetSeed(int new_seed);
int VFastRand();
float VRandom(float min, float max);
void VComputeNormals(VRenderData* rdata);
float VMin(float a, float b);
float VMax(float a, float b);


#endif
