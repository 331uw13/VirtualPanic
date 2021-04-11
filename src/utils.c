#include "libs/gl3w.h"

#include "utils.h"
#include "messages.h"


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


#include <stdio.h>

void VComputeNormals(VRenderData* rdata) {
	if(rdata == NULL) { return; }

	glBindBuffer(GL_ARRAY_BUFFER, rdata->vbo);
	float* vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	if(vertices == NULL) {
		VMessage(VMSG_WARNING, "RenderData vertices are NULL!");
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return;
	}

	int rows = 0;
	for(uint32 i = 0; i < rdata->size; i += 6) {
		printf("%1.2f, %1.2f, %1.2f\n", vertices[i], vertices[i+1], vertices[i+2]);
		rows++;
		if(rows >= 6) {
			rows = 0;
			puts("");
		}
	}



	glUnmapBuffer(GL_ARRAY_BUFFER);
}










