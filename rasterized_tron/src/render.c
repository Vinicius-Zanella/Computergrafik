#include <stdio.h>
#include <GL/gl.h>
#include "../include/world.h"
#include "../include/render.h"

// -- Global --
static fVec2 step = {2.0f / WORLD_SIZE, 2.0f / WORLD_SIZE};

typedef struct {
	GLuint texture;
	int texWidth, texHeight, texChannels;
	const char *filename;
} TexStruct;
//static TexStruct texture = { 0, .filename = "assets/example.png" };

// -- Function declaration --
void initTexture(TexStruct *texture);
void drawPlayer(int p);
void drawTrace(int t);

// --- Entry Point ---
void initRender(void) {
	///TODO Copy paste
	printf("Textures not implemented\n");
	// initTexture(&texture);
}

void resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void render(void) {
	CameraData *camera = getCameraData(1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(camera->rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(camera->rotation.x, 0.0f, 1.0f, 0.0f);
	glRotatef(camera->rotation.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera->position.x, camera->position.y, camera->position.z);

	///TODO remove
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(-1,-1);
		glVertex2f(-1, 1);
		glVertex2f( 1,-1);
		glVertex2f( 1, 1);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	drawPlayer(1);
	drawTrace(1);

	glColor3f(0.0f, 1.0f, 0.0f);
	drawPlayer(2);
	drawTrace(2);

	///TODO consider more plaers(4)
}


// --- Functions ---

void initTexture(TexStruct *texture) {
	///TODO copy paste
}


void drawPlayer(int p) {
	PlayerData *player = getPlayerData(p);
	
	glBegin(GL_POINTS);
		glVertex2f(player->position.x * step.x - 1, player->position.y * step.y - 1);
	glEnd();
}

void drawTrace(int t) {
	PlayerData *player = getPlayerData(t);
	glBegin(GL_LINE_STRIP);
		glVertex2f(player->position.x * step.x - 1, player->position.y * step.y - 1);
		for (int i=player->index; i>=0; i--)
			glVertex2f(player->trace[i].x * step.x - 1, player->trace[i].y * step.y - 1);
	glEnd();
}
