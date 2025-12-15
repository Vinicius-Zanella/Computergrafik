#include <stdio.h>
#include <GL/gl.h>
#include "../include/world.h"
#include "../include/render.h"
#include "../include/texture.h"

// -- Global --
static TexStruct checkersTexture = { 0, .filename = "assets/checkers_2px.png" };

// -- Function declaration --
void drawFloor(void);
void drawPlayer(int p);
void drawTrace(int t);

// --- Entry Point ---
void initRender(void) {
	initTexture(&checkersTexture);
}

void resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void game_render(void) {
	CameraData *camera = getCameraData(1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(camera->rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(camera->rotation.x, 0.0f, 1.0f, 0.0f);
	glRotatef(camera->rotation.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera->position.x, camera->position.y, camera->position.z);
	//glTranslatef(camera->targetPosition.x, camera->targetPosition.y, camera->targetPosition.z);

	///TODO remove
	glColor3f(1.0f, 1.0f, 1.0f);
	drawFloor();

	glColor3f(1.0f, 0.0f, 0.0f);
	drawPlayer(1);
	drawTrace(1);

	glColor3f(0.0f, 1.0f, 0.0f);
	drawPlayer(2);
	drawTrace(2);

	///TODO consider more players(4)
}

// --- Functions ---

void drawFloor(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, checkersTexture.texture);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f( 10,-10); glVertex3f( 0,-1, 0);
		glTexCoord2f( 10, 10); glVertex3f( 0,-1, -WORLD_SIZE);
		glTexCoord2f(-10,-10); glVertex3f(WORLD_SIZE,-1, 0);
		glTexCoord2f(-10, 10); glVertex3f(WORLD_SIZE,-1, -WORLD_SIZE);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawPlayer(int p) {
	PlayerData *player = getPlayerData(p);
	
	glBegin(GL_POINTS);
		//glVertex3f(player->position.x * step.x - 1, 0, -player->position.y * step.y + 1);
		glVertex3f(player->position.x, 0, -player->position.y);		
	glEnd();
}

void drawTrace(int t) {
	PlayerData *player = getPlayerData(t);
	glBegin(GL_LINE_STRIP);
		//glVertex3f(player->position.x * step.x - 1, 0, -player->position.y * step.y + 1);
		glVertex3f(player->position.x, 0, -player->position.y);
		for (int i=player->index; i>=0; i--)
			//glVertex3f(player->trace[i].x * step.x - 1, 0, -player->trace[i].y * step.y + 1);
			glVertex3f(player->trace[i].x, 0, -player->trace[i].y);
	glEnd();
}
