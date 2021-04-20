#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include "libs/gl3w.h"

#include "render.h"
#include "messages.h"
#include "status.h"
#include "internal/core.h"
#include "internal/utils.h"


void VCreateNewShape(VRenderData* rdata, float* points, uint32 size) {
	if(rdata == NULL) { return; }

	glGenVertexArrays(1, &rdata->vao);
	glBindVertexArray(rdata->vao);
	
	glGenBuffers(1, &rdata->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rdata->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STREAM_DRAW);

	const uint32 stride = sizeof(float)*6;

	// points
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	VNullMatrix(&rdata->matrix);
	rdata->size = size/sizeof(float);
	rdata->shader = VGetFirstCreatedShader();
	rdata->texture = 0;
}


void VCreateNewBox(VRenderData* rdata) {
	float box_points[] = {

		// TODO: optimize these.
		
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 0.0f
	};

	VCreateNewShape(rdata, box_points, sizeof box_points);
}


void VCreateNewPlane(VRenderData* rdata) {
	float plane_points[] = {
		
		// TODO: optimize these.

		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f
	};

	VCreateNewShape(rdata, plane_points, sizeof plane_points);
}


void VDestroyRenderData(VRenderData* rdata) {
	glDeleteVertexArrays(1, &rdata->vao);
	glDeleteBuffers(1, &rdata->vbo);
	rdata->vao = 0;
	rdata->vbo = 0;
	rdata->shader = 0;
}


void VSetWireframeEnabled(uint8 b) {
	glPolygonMode(GL_FRONT_AND_BACK, b ? GL_LINE : GL_FILL);
}


void VRender(VRenderData* rdata) {
	if(rdata == NULL) { return; }

	//glUseProgram(rdata->shader);
	VShaderSetMatrix(rdata->shader, "model_matrix", &rdata->matrix);
	VShaderSetVector4(rdata->shader, "material.color", &rdata->material.color);
	VShaderSetFloat(rdata->shader, "material.reflectivity", rdata->material.reflectivity);
	//VShaderSetFloat(rdata->shader, "material.smoothness", &rdata->material.smoothness);
	//VShaderSetFloat(rdata->shader, "material.test", rdata->material.test);
	glBindVertexArray(rdata->vao);
	glDrawArrays(GL_TRIANGLES, 0, rdata->size/6);
}	


