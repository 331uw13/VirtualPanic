#include "libs/gl3w.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "engine.h"
#include "messages.h"
#include "utils.h"
#include "render.h"
#include "light.h"
#include "internal/core.h"
#include "internal/utils.h"


static int engine_status;
static float engine_valuesf[VENGINE_MAX_VALUES];
static VUniformBuffer engine_ubo;
static GLFWwindow* window = NULL;

static void(*engine_setup_callback)() = NULL;
static void(*engine_update_callback)(double) = NULL;
static void VEngineUpdateStatus_I();
static void VEngineUpdatePlayer_I(double delta_time);
static void VEngineUpdateProjection_I();

static VMatrix engine_proj_matrix;
static VMatrix engine_view_matrix;


void VEngineUpdatePlayer_I(double delta_time) {
	VPlayer* plr = VGetPlayer();
	if(plr == NULL) { return; }

	const float speed = plr->speed * delta_time;
	const Vector3 up = { 0.0f, 1.0f, 0.0f };
	VMatrix view;
	
	// Handle player movement.
	
	if(VKeyDown('W')) {
		plr->position.x += plr->camera.direction.x * speed;
		plr->position.y += plr->camera.direction.y * speed;
		plr->position.z += plr->camera.direction.z * speed;
	}
	if(VKeyDown('S')) {
		plr->position.x -= plr->camera.direction.x * speed;
		plr->position.y -= plr->camera.direction.y * speed;
		plr->position.z -= plr->camera.direction.z * speed;
	}
	if(VKeyDown('A')) {
		plr->position.x -= VCross(plr->camera.direction, up).x * speed;
		plr->position.z -= VCross(plr->camera.direction, up).z * speed;
	}
	if(VKeyDown('D')) {
		plr->position.x += VCross(plr->camera.direction, up).x * speed;
		plr->position.z += VCross(plr->camera.direction, up).z * speed;
	}

	if(VKeyDown(VKEY_SPACE)) {
		plr->position.y += speed;
	}
	else if(VKeyDown(VKEY_LSHIFT)) {
		plr->position.y -= speed;
	}
	
	// Update player view matrix and camera.

	const float y = VRadians(plr->camera.yaw);
	const float p = VRadians(plr->camera.pitch);
	Vector3 direction = { 
		cos(y)*cos(p),
		sin(p),
		sin(y)*cos(p)
	};
	plr->camera.direction = direction;
	plr->camera.position = plr->position;
	plr->camera.position.y += plr->height;

	VComputeViewMatrix(&view, plr->camera.position, plr->camera.direction);
	VUniformBufferData(&engine_ubo, &view, sizeof view, 0);
	VUniformBufferData(&engine_ubo, &plr->camera.position, sizeof plr->camera.position, sizeof view * 2);
	
	engine_view_matrix = view;

}


void VEngineUpdateProjection_I() {
	VMatrix proj;

	VComputeProjectionMatrix(&proj, 
			engine_valuesf[VENGINE_FOV],
		   	engine_valuesf[VENGINE_ASPECT_RATIO],
		   	engine_valuesf[VENGINE_ZNEAR],
		   	engine_valuesf[VENGINE_ZFAR]);
	
	VUniformBufferData(&engine_ubo, &proj, sizeof proj, sizeof proj);
	engine_proj_matrix = proj;
}


void VEngineUpdateStatus_I() {
	if((engine_status & VENGINE_INIT_OK) && window != NULL) {
		engine_status |= VENGINE_OK;
	}
	else {
		engine_status &= ~VENGINE_OK;
	}
}


void VEngineInit(const char* title) {
	if(engine_status & VENGINE_INIT_OK) { 
		VMessage(VMSG_WARNING, "%s --> Engine is already setup!", __FUNCTION__);
		return;
	}
	engine_status = 0;

	VMessage(VMSG_INFO, "Initialize glfw... ");
	if(!glfwInit()) {
		VMessage(VMSG_ERROR, "Failed to initialize glfw.");
		return;
	}
	VMessageSameLine("OK");

	int mon_x = 0;
	int mon_y = 0;
	int mon_w = 0;
	int mon_h = 0;

	GLFWmonitor* pm = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(pm, &mon_x, &mon_y, &mon_w, &mon_h);
	
	VMessage(VMSG_INFO, "Create window... ");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(mon_w, mon_h, title, NULL, NULL);
	if(window == NULL) {
		VMessage(VMSG_ERROR, "Failed to create window.");
		VEngineFree();
		return;
	}
	VMessageSameLine("OK");
	
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, mon_x, mon_y);

	VMessage(VMSG_INFO, "Initialize gl3w... ");
	if(gl3wInit()) {
		VMessage(VMSG_ERROR, "Failed to initialize gl3w.");
		VEngineFree();
		return;
	}
	VMessageSameLine("OK");
	VMessage(VMSG_INFO, "OpenGL Version: %s", glGetString(GL_VERSION));

	if(!gl3wIsSupported(4, 3)) {
		VMessage(VMSG_ERROR, "OpenGL 4.3 version is not supported!");
		VEngineFree();
		return;
	}
	
	int fwidth = 0;
	int fheight = 0;
	glfwGetFramebufferSize(window, &fwidth, &fheight);
	glViewport(0, 0, fwidth, fheight);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_STENCIL_TEST);

	// Use high binding point so user can use them starting from 0.
	engine_ubo = VCreateUniformBuffer((sizeof(VMatrix)*2)+sizeof(Vector3), 83);
		
	engine_valuesf[VENGINE_FOV] = 70.f;
	engine_valuesf[VENGINE_ASPECT_RATIO] = (float)mon_w/(float)mon_h;
	engine_valuesf[VENGINE_ZNEAR] = 0.05f;
	engine_valuesf[VENGINE_ZFAR] = 1000.f;
	engine_valuesf[VENGINE_TIME_SCALE] = 1.0f;

	engine_status &= ~VENGINE_INIT_OK;
	VEngineSetCameraEnabled(TRUE);
	VEngineUpdateStatus_I();
	VCoreCompileDefaultVertexModule();
	VCreateNewPlayer();
	VCreateLightUniformBuffer();

	VMessage(VMSG_OK, "Engine is ready!");
	if(engine_setup_callback != NULL) {
		engine_setup_callback();
	}

}


void VEngineStart() {
	if(engine_status & VENGINE_STARTED) {
		VMessage(VMSG_WARNING, "%s --> Engine is already started!", __FUNCTION__);
		return;
	}

	double current_time = glfwGetTime();
	double previous_time = 0.0;
	double mouse_x = 0.0;
	double mouse_y = 0.0;
	double prev_mouse_x = 0.0;
	double prev_mouse_y = 0.0;
	
	engine_status |= VENGINE_STARTED;
	VEngineUpdateProjection_I();
	
	while(!(engine_status & VENGINE_QUIT) && !glfwWindowShouldClose(window)) {

		previous_time = current_time;
		current_time = glfwGetTime();
		const double delta_time = (current_time - previous_time) * engine_valuesf[VENGINE_TIME_SCALE];

		glfwPollEvents();

		prev_mouse_x = mouse_x;
		prev_mouse_y = mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);

		// Check if mouse is moved when camera is enabled.
		// If so set update view bit
		if(engine_status & VENGINE_CAMERA_ENABLED) {
			if(prev_mouse_x != mouse_x || prev_mouse_y != mouse_y) {
				VPlayer* plr = VGetPlayer();
				// x:  -1.0 --> +1.0
				// y:  +1.0 --> -1.0
				plr->camera.yaw += (mouse_x - prev_mouse_x) * plr->camera.sensetivity * delta_time;
				plr->camera.pitch -= (mouse_y - prev_mouse_y) * plr->camera.sensetivity * delta_time;
				if(plr->camera.pitch < -89.9f) {
					plr->camera.pitch = -89.9f;
				}
				else if(plr->camera.pitch > 89.9f) {
					plr->camera.pitch = 89.9f;
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);

		if(engine_update_callback != NULL) {
			engine_update_callback(delta_time);
			VEngineUpdatePlayer_I(delta_time);
		}

		glfwSwapBuffers(window);
	}
	engine_status &= ~VENGINE_STARTED;
}


void VEngineFree() {
	VMessage(VMSG_DEBUG, __FUNCTION__);
	glfwTerminate();
	glfwDestroyWindow(window);
	VDestroyAllRenderData();
	VDestroyUniformBuffer(&engine_ubo);
	VDestroyPlayer();

	window = NULL;
	engine_update_callback = NULL;
	engine_status = 0;
}


void VEngineShutdown() {
	engine_status |= VENGINE_QUIT;
}


int VGetEngineStatus() {
	return engine_status;
}


void VEngineSetVSyncEnabled(uint8 b) {
	glfwSwapInterval(b);
}


void VEngineSetCameraEnabled(uint8 b) {
	glfwSetInputMode(window, GLFW_CURSOR, b ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	b ? (engine_status |= VENGINE_CAMERA_ENABLED) : (engine_status &= ~VENGINE_CAMERA_ENABLED);
}


void VEngineSetf(uint32 flag, float value) {
	if(flag >= VENGINE_MAX_VALUES) {
		VMessage(VMSG_WARNING, "%i --> Invalid flag!", __FUNCTION__);
	}
	else {
		engine_valuesf[flag] = value;
		switch(flag) {
			case VENGINE_FOV:
			case VENGINE_ASPECT_RATIO:
			case VENGINE_ZNEAR:
			case VENGINE_ZFAR:
				VEngineUpdateProjection_I();
				break;

			default: break;
		}
	}
}


float VEngineGetf(uint32 flag) {
	float res = -1.0f;
	if(flag >= VENGINE_MAX_VALUES) {
		VMessage(VMSG_WARNING, "%i --> Invalid flag!", __FUNCTION__);
	}
	else {
		res = engine_valuesf[flag];
	}
	return res;
}


void VEngineSetupCallback(void(*user_callback)()) {
	engine_setup_callback = user_callback;
}


void VEngineUpdateCallback(void(*user_callback)(double)) {
	engine_update_callback = user_callback;
}



// TODO: rename to VKeyInput and pass flag to this and remove VKeyUp.

uint8 VKeyDown(uint32 k) {
	// If k is not printable character just set c to k else make sure its upper case.
	const uint32 c = (k > 0x7E) ? k : (k < 0x61) ? k : k - 0x20;
	return (glfwGetKey(window, c) == GLFW_PRESS);
}


uint8 VKeyUp(uint32 k) {
	const uint32 c = (k > 0x7E) ? k : (k < 0x61) ? k : k - 0x20;
	return (glfwGetKey(window, c) == GLFW_RELEASE);
}

VMatrix VEngineGetProjection() {
	return engine_proj_matrix;
}

VMatrix VEngineGetView() {
	return engine_view_matrix;
}

