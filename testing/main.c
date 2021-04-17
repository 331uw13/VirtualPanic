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

VRenderData* box = NULL;
VRenderData* plane = NULL;

VLight lights[2];
float add = 0.0f;
uint8 keywasdown = 0;
uint8 camera_enabled = 1;

void update(double delta_time) {
	VPlayer* player = VGetPlayer();

	float z = 10.0f*sin(add*3.0f);
	float x = 10.0f*cos(add*3.0f);

	lights[0].position.x = x;
	lights[0].position.z = z;
	lights[1].position.x = -x;
	lights[1].position.z = -z;

	add += 0.03f;

	VUpdateLights(lights, 2);
	

	VRender(box);
	VRender(plane);
	

	if(VKeyDown('T') && !keywasdown) {
		VEngineSetCameraEnabled(camera_enabled =! camera_enabled);
		keywasdown = 1;
	}
	else if(VKeyUp('T')) {
		keywasdown = 0;
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
				" color += vec4(compute_light(lights[1]), 1.0f);"
				" out_color = color;"
			"}"
			);

	box = VCreateNewBox(0.0f, 0.0f, -10.0f);
	plane = VCreateNewPlane(0.0f, -1.0f, 0.0f);

	VComputeNormals(box);
	VComputeNormals(plane);

	float ambience = 0.65f;
	float diffusion = 0.3f;
	float specularity = 0.2f;

	lights[0].position.x = 10.f;
	lights[0].position.y = 2.0f;
	lights[0].position.z = 0.0f;
	lights[0].color.x = 1.0f;
	lights[0].color.y = 0.0f;
	lights[0].color.z = 0.0f;
	lights[0].radius = 15.f;
	lights[0].ambience = ambience;
	lights[0].diffusion = diffusion;
	lights[0].specularity = specularity;

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

	VUpdateLights(lights, 2);


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


