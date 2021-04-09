/*

   This is just only for testing!

*/


#include <stdio.h>
#include "../src/virtual_panic.h"
#include "../src/matrix.h"
#include "../src/libs/gl3w.h"
#include "../src/internal/core.h"
#include "../src/utils.h"
#include <GLFW/glfw3.h>


static VShader shader = 0;

static uint32 vao = 0;
static uint32 vbo = 0;

VRenderData* box = NULL;
VRenderData* plane = NULL;


void update(double delta_time) {
	VPlayer* player = VGetPlayer();

	/*
	VNullMatrix(&test_shape->matrix);
	VMatrixTranslate(&test_shape->matrix, 0.0f, 0.0f, -10.0f);
	*/
	VRender(box);
	VRender(plane);

	if(VKeyDown(VKEY_LCTRL)) {
		player->speed = 20.f;
	}
	else {
		player->speed = VPLAYER_DEFAULT_SPEED;
	}
}


void setup() {

	shader = VCreateShader(
			"#version 430 core\n"

			"out vec4 out_color;"
			"void main() {"
				"out_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
			"}"
			);

	box = VCreateNewBox(0.0f, 0.0f, -10.0f);
	plane = VCreateNewPlane(0.0f, -1.0f, 0.0f);

	VMatrixScale(&plane->matrix, 50.f, 0.0f, 50.f);


	VEngineStart();
}


int main() {
	VEngineSetupCallback(setup);
	VEngineUpdateCallback(update);	
	VEngineInit("Testing libVirtualPanic");
	
	VDestroyShader(&shader);
	VEngineFree();
	
	return 0;
}


