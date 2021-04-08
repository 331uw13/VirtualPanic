#include <stddef.h>
#include "utils.h"
#include "matrix.h"


void VNullMatrix(VMatrix* m) {
	if(m == NULL) { return; }
	m->data[0][0] = 1.0f;
	m->data[0][1] = 0.0f;
	m->data[0][2] = 0.0f;
	m->data[0][3] = 0.0f;
	
	m->data[1][0] = 0.0f;
	m->data[1][1] = 1.0f;
	m->data[1][2] = 0.0f;
	m->data[1][3] = 0.0f;

	m->data[2][0] = 0.0f;
	m->data[2][1] = 0.0f;
	m->data[2][2] = 1.0f;
	m->data[2][3] = 0.0f;

	m->data[3][0] = 0.0f;
	m->data[3][1] = 0.0f;
	m->data[3][2] = 0.0f;
	m->data[3][3] = 1.0f;
}

void VMatrixScale(VMatrix* dest, float x, float y, float z) {
	if(dest == NULL) { return; }
	dest->data[0][0] *= x;
	dest->data[0][1] *= x;
	dest->data[0][2] *= x;
	dest->data[0][3] *= x;
	
	dest->data[1][0] *= y;
	dest->data[1][1] *= y;
	dest->data[1][2] *= y;
	dest->data[1][3] *= y;
	
	dest->data[2][0] *= z;
	dest->data[2][1] *= z;
	dest->data[2][2] *= z;
	dest->data[2][3] *= z;
}

void VMatrixTranslate(VMatrix* dest, float x, float y, float z) {
	if(dest == NULL) { return; }
	dest->data[3][0] = dest->data[0][0]*x + dest->data[1][0]*y + dest->data[2][0]*z + dest->data[3][0];
	dest->data[3][1] = dest->data[0][1]*x + dest->data[1][1]*y + dest->data[2][1]*z + dest->data[3][1];
	dest->data[3][2] = dest->data[0][2]*x + dest->data[1][2]*y + dest->data[2][2]*z + dest->data[3][2];
	dest->data[3][3] = dest->data[0][3]*x + dest->data[1][3]*y + dest->data[2][3]*z + dest->data[3][3];
}

void VComputeProjectionMatrix(VMatrix* dest, float fov, float aspect_ratio, float z_near, float z_far) {
	if(dest == NULL) { return; }
	VNullMatrix(dest);

	const float f = tan(VRadians(fov)/2.0f);
	const float zd = z_far-z_near;

	dest->data[0][0] = 1.0f/(f*aspect_ratio);
	dest->data[1][1] = 1.0f/f;
	dest->data[2][2] = -(z_far+z_near)/zd;
	dest->data[3][2] = -(2.0f*z_far*z_near)/zd;
	dest->data[2][3] = -1.0f;
	dest->data[3][3] = 0.0f;
}

void VComputeViewMatrix(VMatrix* dest, Vector3 position, Vector3 direction) {
	if(dest == NULL) { return; }
	VNullMatrix(dest);

	const Vector3 up = { 0.0f, 1.0f, 0.0f };
	Vector3 center = { position.x+direction.x, position.y+direction.y, position.z+direction.z };
	Vector3 f = { center.x-position.x, center.y-position.y, center.z-position.z };
	f = VNormalized(f);

	Vector3 s = VCross(f, up);
	s = VNormalized(s);

	const Vector3 u = VCross(s, f);

	dest->data[0][0] = s.x;
	dest->data[1][0] = s.y;
	dest->data[2][0] = s.z;

	dest->data[0][1] = u.x;
	dest->data[1][1] = u.y;
	dest->data[2][1] = u.z;

	dest->data[0][2] = -f.x;
	dest->data[1][2] = -f.y;
	dest->data[2][2] = -f.z;

	dest->data[3][0] = -VDot(s, position);
	dest->data[3][1] = -VDot(u, position);
	dest->data[3][2] =  VDot(f, position);
}


