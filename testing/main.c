
// This is just only for testing!

#include <stdio.h>
#include "../src/virtual_panic.h"
#include "../src/matrix.h"
#include "../src/libs/gl3w.h"
#include "../src/internal/core.h"
#include "../src/utils.h"



static uint32 shader = 0;

static uint32 vao = 0;
static uint32 vbo = 0;



void update(double delta_time) {

	VPlayer* player = VGetPlayer();

	glUseProgram(shader);
	VMatrix proj;
	
	VMatrix model;
	VNullMatrix(&model);

	VCreateProjectionMatrix(&proj, 70.f, 1920.f/1080.f, 0.1, 500.f);
	VMatrixTranslate(&model, 0.0f, 0.0f, -10.0f);

	glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, &proj.data[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &player->camera.matrix.data[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model.data[0][0]);

	glBindVertexArray(vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//GLenum error = glGetError();
	//printf("error: %i | camera: %1.2f, %1.2f, %1.2f\n", error, cam_front.x, cam_front.y, cam_front.z);

}


void setup() {
	
	shader = VLoadShader(
			"#version 430 core\n"

			"out vec4 out_color;"
			"void main() {"
				"out_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
			"}"
			);

	float points[] = {
	    -2.0f, -2.0f, -2.0f,
         2.0f, -2.0f, -2.0f,
         2.0f,  2.0f, -2.0f,
         2.0f,  2.0f, -2.0f,
        -2.0f,  2.0f, -2.0f,
        -2.0f, -2.0f, -2.0f,

        -2.0f, -2.0f,  2.0f,
         2.0f, -2.0f,  2.0f,
         2.0f,  2.0f,  2.0f,
         2.0f,  2.0f,  2.0f,
        -2.0f,  2.0f,  2.0f,
        -2.0f, -2.0f,  2.0f,

        -2.0f,  2.0f,  2.0f,
        -2.0f,  2.0f, -2.0f,
        -2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f, -2.0f,
        -2.0f, -2.0f,  2.0f,
        -2.0f,  2.0f,  2.0f,

         2.0f,  2.0f,  2.0f,
         2.0f,  2.0f, -2.0f,
         2.0f, -2.0f, -2.0f,
         2.0f, -2.0f, -2.0f,
         2.0f, -2.0f,  2.0f,
         2.0f,  2.0f,  2.0f,

        -2.0f, -2.0f, -2.0f,
         2.0f, -2.0f, -2.0f,
         2.0f, -2.0f,  2.0f,
         2.0f, -2.0f,  2.0f,
        -2.0f, -2.0f,  2.0f,
        -2.0f, -2.0f, -2.0f,

        -2.0f,  2.0f, -2.0f,
         2.0f,  2.0f, -2.0f,
         2.0f,  2.0f,  2.0f,
         2.0f,  2.0f,  2.0f,
        -2.0f,  2.0f,  2.0f,
        -2.0f,  2.0f, -2.0f
	};


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)/**sizeof(float)*/, points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
	glEnableVertexAttribArray(0);

	VEngineStart();
}


int main() {

	VEngineSetupCallback(setup);
	VEngineUpdateCallback(update);	
	VEngineInit("Testing libVirtualPanic");
	

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
	VUnloadShader(&shader);
	VEngineFree();
	
	return 0;
}


