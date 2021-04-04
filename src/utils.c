#include "utils.h"


float VRadians(float a) {
	return a*(M_PI/180.f);
}

float VLength(Vector3 v) {
	return sqrt(VDot(v, v));
}

float VDot(Vector3 v0, Vector3 v1) {
	const float tx = v0.x*v1.x;
	const float ty = v0.y*v1.y;
	const float tz = v0.z*v1.z;
	return tx+ty+tz;
}

Vector3 VNormalized(Vector3 v) {
	const float l = VLength(v);
	Vector3 norm = { v.x/l, v.y/l, v.z/l };
	return norm;
}

Vector3 VCross(Vector3 v0, Vector3 v1) {
	const float x = v0.y*v1.z - v0.z*v1.y;
	const float y = v0.z*v1.x - v0.x*v1.z;
	const float z = v0.x*v1.y - v0.y*v1.x;
	Vector3 c = { x, y, z };
	return c;
}


