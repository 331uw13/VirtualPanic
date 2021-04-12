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

	// Loop through each triangle and compute normals for them.

	glBindBuffer(GL_ARRAY_BUFFER, rdata->vbo);
	float* vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	if(vertices == NULL) {
		VMessage(VMSG_WARNING, "RenderData vertices are NULL!");
		glUnmapBuffer(GL_ARRAY_BUFFER);
		return;
	}

	Vector3 triangle[3];
	int k = 0;
	
	for(uint32 i = 0; i < rdata->size; i += 6) {
		const Vector3 current_point = { vertices[i], vertices[i+1], vertices[i+2] };
		triangle[k] = current_point;

		k++;

		if(k >= 3) {
			const Vector3 a = { 
				triangle[1].x - triangle[0].x,
				triangle[1].y - triangle[0].y,
				triangle[1].z - triangle[0].z
			};

			const Vector3 b = { 
				triangle[2].x - triangle[0].x,
				triangle[2].y - triangle[0].y,
				triangle[2].z - triangle[0].z
			};

			const Vector3 p = VCross(a, b);
			const Vector3 normal = VNormalized(p);

			// point 2 normals
			vertices[i+3] = -normal.x;
			vertices[i+4] = -normal.y;
			vertices[i+5] = -normal.z;
			
			// point 1 normals
			vertices[i-3] = -normal.x;
			vertices[i-2] = -normal.y;
			vertices[i-1] = -normal.z;
			
			// point 0 normals
			vertices[i-9] = -normal.x;
			vertices[i-8] = -normal.y;
			vertices[i-7] = -normal.z;

			k = 0;	
		}
		//printf("%1.2f, %1.2f, %1.2f\n", vertices[i], vertices[i+1], vertices[i+2]);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
}










