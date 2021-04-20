/*

   This is just only for testing!

*/


#include <stdio.h>
#include "../src/virtual_panic.h"
#include "../src/matrix.h"
#include "../src/libs/gl3w.h"
#include "../src/internal/core.h"
#include "../src/utils.h"
#include "../src/light.h"
#include <GLFW/glfw3.h>


static VShader shader = 0;


VRenderData box;
VRenderData plane;

VLight lights[2];
float add = 0.0f;
uint8 keywasdown = 0;
uint8 key2wasdown = 0;
uint8 camera_enabled = 1;
uint8 follow_light = 1;

void update(double delta_time) {
	VPlayer* player = VGetPlayer();

	if(follow_light) {
		lights[0].position.x = player->camera.position.x;
		lights[0].position.y = player->camera.position.y;
		lights[0].position.z = player->camera.position.z;
		VUpdateLights(lights, 0, 1);
	}

	add += 0.1f;

	float f = 2.f*sin(add);
	if(f < 0.0f) {
		f = -f;
	}

	VNullMatrix(&box.matrix);
	VMatrixScale(&box.matrix, f, f, f);
	VMatrixTranslate(&box.matrix, 0.0f, 0.0f, 0.0f);


	VRender(&box);
	VRender(&plane);

	if(VKeyDown('T') && !keywasdown) {
		VEngineSetCameraEnabled(camera_enabled = !camera_enabled);
		keywasdown = 1;
	}
	else if(VKeyUp('T')) {
		keywasdown = 0;
	}
	if(VKeyDown('E') && !key2wasdown) {
		follow_light = !follow_light;
		key2wasdown = 1;
	}
	else if(VKeyUp('E')) {
		key2wasdown = 0;
	}

	if(VKeyDown(VKEY_LCTRL)) {
		player->speed = 20.f;
	}
	else {
		player->speed = VPLAYER_DEFAULT_SPEED;
	}
}


void setup() {

	shader = VCreateShader(
			"out vec4 out_color;"
			"void main() {"
				" vec4 color = vec4(0.0f);"
				" color += vec4(compute_light(lights[0]), 1.0f);"
				//" color += vec4(compute_light(lights[1]), 1.0f);"
				" out_color = color;"
			"}"
			);

	VCreateNewBox(&box);
	VCreateNewPlane(&plane);
	VMatrixTranslate(&box.matrix, 0.0f, 0.0f, -10.f);
	VMatrixTranslate(&plane.matrix, 0.0f, -1.0f, 0.0f);
	VMatrixScale(&plane.matrix, 50.f, 0.0f, 50.f);
	
	VComputeNormals(&box);
	VComputeNormals(&plane);

	box.material.color.x = 0.1f;
	box.material.color.y = 1.0f;
	box.material.color.z = 0.1f;
	
	plane.material.color.x = 0.9f;
	plane.material.color.y = 0.9f;
	plane.material.color.z = 0.9f;


	plane.material.reflectivity = 64.f;
	box.material.reflectivity = 10.0f;


	float ambience = 0.65f;
	float diffusion = 0.3f;
	float specularity = 0.2f;

	lights[0].position.x = 10.f;
	lights[0].position.y = 2.0f;
	lights[0].position.z = 0.0f;
	lights[0].color.x = 1.0f;
	lights[0].color.y = 1.0f;
	lights[0].color.z = 1.0f;
	lights[0].radius = 15.f;
	lights[0].ambience = ambience;
	lights[0].diffusion = diffusion;
	lights[0].specularity = specularity;
/*
	lights[1].position.x = -10.f;
	lights[1].position.y = 2.0f;
	lights[1].position.z = 0.0f;
	lights[1].color.x = 0.0f;
	lights[1].color.y = 1.0f;
	lights[1].color.z = 1.0f;
	lights[1].radius = 15.f;
	lights[1].ambience = ambience;
	lights[1].diffusion = diffusion;
	lights[1].specularity = specularity;
*/
	VUpdateLights(lights, 0, 1);
	VEngineStart();

	VDestroyRenderData(&box);
	VDestroyRenderData(&plane);
}


int main() {
	VEngineSetupCallback(setup);
	VEngineUpdateCallback(update);	
	VEngineInit("Testing libVirtualPanic");
	
	VDestroyShader(&shader);
	VEngineFree();
	
	return 0;
}


