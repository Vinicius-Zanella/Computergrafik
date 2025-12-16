#include <stdio.h>
#include <GL/gl.h>
#include "../include/world.h"
#include "../include/render.h"
#include "../include/texture.h"

// -- Global --
int windowWidth = 1;
int windowHeight = 1;
static TexStruct checkersTexture = { 0, .filename = "assets/checkers_2px.png" };
int playerCount = 0;
struct displayArea *playerViewports;

struct displayArea displayPositions[] = {
	{{ 0.0f, 0.0f},{ 1.0f, 1.0f}},
	{{ 0.0f, 0.5f},{ 1.0f, 1.0f}},
	{{ 0.0f, 0.0f},{ 1.0f, 0.5f}},
	{{ 0.0f, 0.0f},{ 0.5f, 1.0f}},
	{{ 0.5f, 0.0f},{ 1.0f, 1.0f}},
	{{ 0.5f, 0.5f},{ 1.0f, 1.0f}},
	{{ 0.5f, 0.0f},{ 1.0f, 0.5f}},
	{{ 0.0f, 0.0f},{ 0.5f, 0.5f}},
	{{ 0.0f, 0.5f},{ 0.5f, 1.0f}},
};

// -- Function declaration --
void drawFloor(void);
void drawPlayer(int p);
void drawTrace(int t);
void renderPlayer(int player);

// --- Entry Point ---
void initRender(int _playerCount, int width, int height, struct displayArea *viewports) {
	initTexture(&checkersTexture);
	playerCount = _playerCount;
	windowWidth = width;
	windowHeight = height;
	playerViewports = viewports;
}

void resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);	///TODO: remove?
	windowWidth = width;
	windowHeight = height;
}

void game_render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int c=0; c<playerCount; c++) {
		renderPlayer(c);
	}
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

void renderPlayer(int player) {
	CameraData *camera = getCameraData(player);

	glViewport(
		playerViewports[player].start.x * windowWidth,
		playerViewports[player].start.y * windowHeight,
		playerViewports[player].end.x * windowWidth,
		playerViewports[player].end.y * windowHeight);
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

	for(int c=1; c<=playerCount; c++) {
		glColor3f(1.0f, 0.0f, 0.0f);	///TODO: get correct colour
		drawPlayer(c);
		drawTrace(c);	
	}
}
