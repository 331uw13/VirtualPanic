#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include "libs/gl3w.h"

#include "render.h"
#include "messages.h"
#include "status.h"
#include "internal/core.h"


static VRenderData** renderable_data = NULL;
static uint32 renderable_data_size = 0;


VRenderData* VCreateEmptyRenderData() {
	VMessage(VMSG_DEBUG, __FUNCTION__);
	
	VRenderData* res = NULL;
	VRenderData** tmpdata_ptr = NULL;
	
	// First check if there is any free space left for use.
	uint8 found_free = 0;
	uint32 next_free_index = 0;

	for(uint32 i = 0; i < renderable_data_size; i++) {
		if(renderable_data[i] == NULL) {
			found_free = 1;
			next_free_index = i;
			break;
		}
	}	
	
	if(!found_free) {
		// No free space were found so allocate more.
		// When renderable_data is equal to NULL this realloc call will be equal to malloc(sizeof *renderable_data)
		tmpdata_ptr = realloc(renderable_data, sizeof *renderable_data * (renderable_data_size+1));
	}

	if(found_free || tmpdata_ptr != NULL) {
		renderable_data = tmpdata_ptr;
		res = malloc(sizeof **renderable_data);
		if(res != NULL) {
			res->id = renderable_data_size;
			res->vao = 0;
			res->vbo = 0;
			res->shader = VGetFirstCreatedShader();
			res->texture = 0;
			res->points = 0;
			res->wireframe = 0;
			VNullMatrix(&res->matrix);
			glGenVertexArrays(1, &res->vao);
			glGenBuffers(1, &res->vbo);

			if(found_free) {
				renderable_data[next_free_index] = res;
			}
			else {
				renderable_data[renderable_data_size] = res;
				renderable_data_size++;
			}
		}
	   	else {
			VMessage(VMSG_ERROR, "Failed to allocate memory for new render data!");
			if(errno == ENOMEM) {
				VMessage(VMSG_ERROR, "Out of memory!");
			}

			if(!found_free && renderable_data_size > 0) {
				// Shrink 'renderable_data' by one because it is not needed anymore.
				// I dont how this could go wrong but just to be safe from memory leaks anyway.
				tmpdata_ptr = NULL;
				tmpdata_ptr = realloc(renderable_data, sizeof *renderable_data * (renderable_data_size-1));
				if(tmpdata_ptr != NULL) {
					renderable_data = tmpdata_ptr;
				}
			}
	   	}
	}
	else {
		VMessage(VMSG_ERROR, "Failed to allocate memory for new render data!");
		if(errno == ENOMEM) {
			VMessage(VMSG_ERROR, "Out of memory!");
		}
   	}

	return res;
}


VRenderData* VGetRenderData(uint32 id) {
	VRenderData* res = NULL;
	if(id < renderable_data_size) {
		res = renderable_data[id];
	}
	return res;
}


uint32 VGetRenderDataCount() {
	return renderable_data_size;
}


void VDestroyRenderData(VRenderData* box) {
	if(box == NULL) { return; }

	// TODO: try make this better.
	// Shift remaining items to left and shrink renderable_data by one.

	const uint32 boxid = box->id;
	glDeleteVertexArrays(1, &box->vao);
	glDeleteBuffers(1, &box->vbo);
	
	free(renderable_data[boxid]);
	renderable_data[boxid] = NULL;

	if(renderable_data_size <= 1) {
		free(renderable_data);
		renderable_data = NULL;
		renderable_data_size = 0;
	} else {
		// Shift items from right to left starting from now destroyed box index
		// and update item index.
		for(uint32 i = boxid+1; i < renderable_data_size; i++) {
			VRenderData* src = renderable_data[i];
			VRenderData* dest = renderable_data[i-1];
			if(src == NULL) {
				continue;
		   	}
			dest = src;
			dest->id = i-1; // i should probably rename id to index...
			renderable_data[i-1] = dest;
		}

		VRenderData** tmpdata_ptr = realloc(renderable_data, sizeof *renderable_data * (renderable_data_size-1));
		if(tmpdata_ptr != NULL) {
			renderable_data = tmpdata_ptr;
			renderable_data_size--;
		}
	}
}


void VDestroyAllRenderData() {
	if(renderable_data == NULL) { return; }
	for(uint32 i = 0; i < renderable_data_size; i++) {
		if(renderable_data[i] == NULL) { 
			continue;
		}
		VMessage(VMSG_DEBUG, "%s --> Free %p", __FUNCTION__, renderable_data[i]);
		free(renderable_data[i]);
		renderable_data[i] = NULL;
	}
	free(renderable_data);
	renderable_data = NULL;
	renderable_data_size = 0;
}


VRenderData* VCreateNewShape(float* points, uint32 size) {
	VRenderData* rdata = VCreateEmptyRenderData();
	if(rdata != NULL) {
		glBindVertexArray(rdata->vao);
		glBindBuffer(GL_ARRAY_BUFFER, rdata->vbo);
		glBufferData(GL_ARRAY_BUFFER, size, points, GL_STREAM_DRAW);
		float stride = sizeof(float)*6;
		
		// Points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
		glEnableVertexAttribArray(0);
		
		// Normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		rdata->points = (size/sizeof(float))/6;

		rdata->size = size/sizeof(float);
	}

	return rdata;
}


VRenderData* VCreateNewBox(float initial_x, float initial_y, float initial_z) {
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
	
	VRenderData* rdata = VCreateNewShape(box_points, sizeof box_points);
	if(rdata != NULL) {
		VMatrixTranslate(&rdata->matrix, initial_x, initial_y, initial_z);
	}
	return rdata;
}

VRenderData* VCreateNewPlane(float initial_x, float initial_y, float initial_z) {
	float plane_points[] = {
		
		// TODO: optimize these.

		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f
	};
	
	VRenderData* rdata = VCreateNewShape(plane_points, sizeof plane_points);
	if(rdata != NULL) {
		VMatrixTranslate(&rdata->matrix, initial_x, initial_y, initial_z);
	}
	return rdata;
}


void VRender(VRenderData* rdata) {
	//if(rdata == NULL) { return; }
	glUseProgram(rdata->shader);
	VShaderSetMatrix(rdata->shader, "model_matrix", &rdata->matrix);
	glBindVertexArray(rdata->vao);
	glDrawArrays(GL_TRIANGLES, 0, rdata->points);
}	





