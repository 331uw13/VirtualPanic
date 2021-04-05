#include "libs/gl3w.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine.h"
#include "messages.h"
#include "utils.h"
#include "internal/core.h"
#include "internal/utils.h"


static int engine_status = 0;
static int engine_time_scale = 1;
static GLFWwindow* window = NULL;
static VPlayer* current_player = NULL;


static void(*engine_setup_callback)() = NULL;
static void(*engine_update_callback)(double) = NULL;
static void VEngineUpdateStatus_I();
static void VEngineUpdatePlayer_I(double delta_time);
static void VEngineUpdateView_I();

/*


    Some random notes:

	Optimize normals and texture coordinates to save memory.

	

    DrawableData
   		Save vertex array object (vao)
		Save vertex buffer object (vbo)
		Save texture.
		Save shader id for rendering.
		Dont save matrix here because some stuff wont need it.


	Shape
		DrawableData
		Color can be changed.
		Needs matrix

		Create new shape:
			VShape* shape = VCreateNewShape(vertices, count);


	Planes
		DrawableData
		Color can be changed.
		Needs matrix.

		Create new plane:
			VPlane* plane = VCreateNewPlane(width, height);

		Draw one plane:
			VRenderPlane(plane);

		Draw planes instanced:
			VRenderPlanesInstanced(plane, matrices, count);

		Destroy it
			VDestroyPlane(plane);



	Box
		DrawableData
		Color can be changed.
		Needs matrix.

		Create new box:
			VBox* box = VCreateNewBox();

		Draw one box:
			VRenderBox(box);

		Draw boxes instanced:
			VRenderBoxesInstanced(box, matrices, count);
		
 		Destroy it
			VDestroyBox(box);


	Line
		DrawableData
		String force can be applied.
		Color can be changed.
		Doesnt need matrix.

		Create new line:
		  VLine* line = VCreateNewLine();

		Draw one line:
		  VRenderLine(line);

		Draw lines instanced:
		  VRenderLinesInstanced(lines, Vector3 points, count);

		Destoy it
			VDestroyLine(line);


	Terrain
		DrawableData
		Color can be changed.
		Terrain can be resized.
		Give height map to shader and use geometry shader?
		Doesnt need matrix.

		Create new terrain:
		  VTerrain* terrain = VCreateNewTerrain();

		Draw terrain:
		  VRenderTerrain(terrain);

		Destroy it
			VDestroyTerrain(terrain);


*/

void VEngineUpdatePlayer_I(double delta_time) {
	uint8 did_move = 0;
	const float speed = current_player->speed * delta_time;
	const Vector3 up = { 0.0f, 1.0f, 0.0f };
	VCamera* cam = &current_player->camera;
	
	if(VKeyDown('W')) {
		did_move = 1;
		current_player->position.x += cam->direction.x * speed;
		current_player->position.y += cam->direction.y * speed;
		current_player->position.z += cam->direction.z * speed;
	}
	if(VKeyDown('S')) {
		did_move = 1;
		current_player->position.x -= cam->direction.x * speed;
		current_player->position.y -= cam->direction.y * speed;
		current_player->position.z -= cam->direction.z * speed;
	}
	if(VKeyDown('A')) {
		did_move = 1;
		current_player->position.x -= VCross(cam->direction, up).x * speed;
		current_player->position.z -= VCross(cam->direction, up).z * speed;
	}
	if(VKeyDown('D')) {
		did_move = 1;
		current_player->position.x += VCross(cam->direction, up).x * speed;
		current_player->position.z += VCross(cam->direction, up).z * speed;
	}

	if(did_move) {
		VSetSFlag(&engine_status, VENGINE_UPDATE_VIEW, TRUE);
	}
}



// NOTE: rename to VEngineUpdatePlayerView() ?
void VEngineUpdateView_I() {
	VCamera* cam = &current_player->camera;

	const float y = VRadians(cam->yaw);
	const float p = VRadians(cam->pitch);
	Vector3 direction = { 
		cos(y)*cos(p),
		sin(p),
		sin(y)*cos(p)
	};
	cam->direction = direction;
	cam->position = current_player->position;
	cam->position.y += current_player->height;

	VComputeViewMatrix(&current_player->camera.matrix, cam->position, cam->direction);
}



void VEngineUpdateStatus_I() {
	VSetSFlag(&engine_status, VENGINE_OK, window != NULL &&
			!(engine_status & VENGINE_QUIT) && (engine_status & VENGINE_INIT_OK));
}



void VEngineInit(const char* title) {
	if(engine_status & VENGINE_INIT_OK) { 
		VMessage(VMSG_WARNING, "%s --> Engine is already setup!", __FUNCTION__);
		return;
	}
	engine_status = 0;
	engine_time_scale = 1;

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
	glfwSwapInterval(1);

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
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_STENCIL_TEST);

	if(current_player == NULL) {
		current_player = (VPlayer*)malloc(sizeof(VPlayer));
		current_player->speed = 8.0f;
		current_player->gravity = 0.0f; // todo
		current_player->height = 4.0f;
		current_player->position.x = 0.0f;
		current_player->position.y = 0.0f;
		current_player->position.z = 0.0f;
		current_player->camera.position.x = 0.0f;
		current_player->camera.position.y = current_player->height;
		current_player->camera.position.z = 0.0f;
		current_player->camera.sensetivity = 7.0f;
		current_player->camera.yaw = -90.f;
		current_player->camera.pitch = 0.0f;
		current_player->camera.noclip = FALSE; // todo
	}
	
	VSetSFlag(&engine_status, VENGINE_INIT_OK, TRUE);
	VEngineSetCameraEnabled(TRUE);
	VEngineUpdateStatus_I();

	VCoreCompileDefaultVertexModule();
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
	
	VSetSFlag(&engine_status, VENGINE_UPDATE_VIEW, TRUE);
	VSetSFlag(&engine_status, VENGINE_STARTED, TRUE);

	while(!(engine_status & VENGINE_QUIT) && !glfwWindowShouldClose(window)) {

		previous_time = current_time;
		current_time = glfwGetTime();
		const double delta_time = (current_time - previous_time) * engine_time_scale;

		glfwPollEvents();
		VEngineUpdatePlayer_I(delta_time);

		prev_mouse_x = mouse_x;
		prev_mouse_y = mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);

		if(engine_status & VENGINE_CAMERA_ENABLED) {
			if(prev_mouse_x != mouse_x || prev_mouse_y != mouse_y) {
				// x:  -1.0 --> +1.0
				// y:  +1.0 --> -1.0
				current_player->camera.yaw += (mouse_x - prev_mouse_x)*current_player->camera.sensetivity*delta_time;
				current_player->camera.pitch -= (mouse_y - prev_mouse_y)*current_player->camera.sensetivity*delta_time;
				VSetSFlag(&engine_status, VENGINE_UPDATE_VIEW, TRUE);
			}
		}

		if(engine_status & VENGINE_UPDATE_VIEW) {
			VEngineUpdateView_I();
			VSetSFlag(&engine_status, VENGINE_UPDATE_VIEW, FALSE);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);


		if(engine_update_callback != NULL) {
			engine_update_callback(delta_time);
		}
	
		glfwSwapBuffers(window);
	}
}



void VEngineFree() {
	VMessage(VMSG_DEBUG, __FUNCTION__);
	glfwTerminate();
	glfwDestroyWindow(window);
	free(current_player);

	current_player = NULL;
	window = NULL;
	engine_update_callback = NULL;
	engine_status = 0;
}



void VEngineShutdown() {
	VSetSFlag(&engine_status, VENGINE_QUIT, TRUE);
}



int VGetEngineStatus() {
	return engine_status;
}



void VEngineSetCameraEnabled(uint8 b) {
	glfwSetInputMode(window, GLFW_CURSOR, b ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	VSetSFlag(&engine_status, VENGINE_CAMERA_ENABLED, b);
}



void VEngineSetTimeScale(uint32 time_scale) {
	engine_time_scale = time_scale;
}



void VEngineSetupCallback(void(*user_callback)()) {
	engine_setup_callback = user_callback;
}



void VEngineUpdateCallback(void(*user_callback)(double)) {
	engine_update_callback = user_callback;
}



VPlayer* VGetPlayer() {
	return current_player;
}



uint8 VKeyDown(char c) {
	return (glfwGetKey(window, c) == GLFW_PRESS);
}



uint32 VLoadShader(const char* src) {
	if(src == NULL) { return 0; }
	uint32 shader = 0;
	uint32 module_id = VCoreCompileShaderModule(src, GL_FRAGMENT_SHADER, VCORE_COMPILE_USER_SHADER);
	if(module_id != 0) {
		shader = VCoreLinkShaderModule(module_id);
	}

	return shader;
}



void VUnloadShader(uint32* id) {
	if(id == NULL) { return; }
	glDeleteProgram(*id);
	*id = 0;
}


