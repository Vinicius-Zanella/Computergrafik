#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>
#include "../include/world.h"
#include "../include/render.h"
#include "../include/texture.h"

// -- Global --
static int windowWidth = 1;
static int windowHeight = 1;
static TexStruct checkersTexture = { 0, .filename = "assets/checkers_2px_dark.png" };
static TexStruct lightCycleTexture = { 0, .filename = "assets/light_cycle_5-400px.png"};
static TexStruct lightTraceTexture = { 0, .filename = "assets/light-bike_trace_128.png"};
static int countPlayers = 0;
struct displayArea *playerViewports;

struct displayArea displayPositions[] = {
	{{ 0.0f, 0.0f},{ 1.0f, 1.0f}},
	{{ 0.0f, 0.5f},{ 1.0f, 0.5f}},
	{{ 0.0f, 0.0f},{ 1.0f, 0.5f}},
	{{ 0.0f, 0.0f},{ 0.5f, 1.0f}},
	{{ 0.5f, 0.0f},{ 0.5f, 1.0f}},
	{{ 0.5f, 0.5f},{ 0.5f, 0.5f}},
	{{ 0.5f, 0.0f},{ 0.5f, 0.5f}},
	{{ 0.0f, 0.0f},{ 0.5f, 0.5f}},
	{{ 0.0f, 0.5f},{ 0.5f, 0.5f}},
};

// -- Function declaration --
void drawFloor(void);
void drawTrace(int t);
void renderPlayer(int player);

// --- Entry Point ---
void initRender(int _playerCount, int width, int height, struct displayArea *viewports) {
	initTexture(&checkersTexture);
	initTexture(&lightCycleTexture);
	initTexture(&lightTraceTexture);
	countPlayers = _playerCount;
	windowWidth = width;
	windowHeight = height;
	playerViewports = viewports;
}

void render_resize(GLFWwindow *window, int width, int height) {
	(void)window;
	
	windowWidth = width;
	windowHeight = height;
}

void game_render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int c=0; c<countPlayers; c++) {
		renderPlayer(c);
	}
}

// --- Functions ---

void drawFloor(void) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, checkersTexture.texture);
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f( 1,-1); glVertex3f( 0,-1, 0);
		glTexCoord2f( 1, 1); glVertex3f( 0,-1, -WORLD_SIZE);
		glTexCoord2f(-1,-1); glVertex3f(WORLD_SIZE,-1, 0);
		glTexCoord2f(-1, 1); glVertex3f(WORLD_SIZE,-1, -WORLD_SIZE);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBike(int p, int c) {
	PlayerData *player = getPlayerData(p);
	CameraData *camera = getCameraData(c);

	//calculate angle
	float angle = atan2(
		player->position.x + camera->position.x,
		player->position.y - camera->position.z
	);

	// Adjust parameters
	float facing = angle;
	angle = angle * (180 / M_PI);

	if(camera->position.z <= player->position.y) { angle = 180 + angle; }
	
	angle -= 90 * player->direction;
	if (angle > 360) angle -= 360;
	if (angle < 0) angle += 360;

	int sprite;
	if(angle < 22.5f) {
		sprite = 0;
	} else
	if(angle < 67.5f) {
		sprite = 1;
	} else
	if(angle < 112.5f) {
		sprite = 2;
	} else
	if(angle < 157.5f) {
		sprite = 3;
	} else
	if(angle < 202.5f) {
		sprite = 4;
	} else
	if(angle < 247.5f) {
		sprite = 3;
	} else
	if(angle < 292.5f) {
		sprite = 2;
	} else
	if(angle < 337.5f) {
		sprite = 1;
	} else {
		sprite = 0;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightCycleTexture.texture);

	int width = 6;
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.2f + sprite * 0.2f,	1); glVertex3f( player->position.x - width * cos(facing), -1, -player->position.y - width * sin(facing));
		glTexCoord2f(0 + sprite * 0.2f, 	1); glVertex3f( player->position.x + width * cos(facing), -1, -player->position.y + width * sin(facing));
		glTexCoord2f(0.2f + sprite * 0.2f, 	0);	glVertex3f( player->position.x - width * cos(facing),  3, -player->position.y - width * sin(facing));
		glTexCoord2f(0 + sprite * 0.2f, 	0);	glVertex3f( player->position.x + width * cos(facing),  3, -player->position.y + width * sin(facing));
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawTrace(int t) {
	PlayerData *player = getPlayerData(t);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lightTraceTexture.texture);

	glBegin(GL_TRIANGLE_STRIP);
		fVec2 pos = { player->position.x, -player->position.y };
		for (int i=player->index; i>=0; i--) {
			glTexCoord2f(0, 0); glVertex3f(pos.x, -1, pos.y);
			glTexCoord2f(1, 0); glVertex3f(player->trace[i].x, -1, -player->trace[i].y);
			glTexCoord2f(0, 1); glVertex3f(pos.x, 2, pos.y);
			glTexCoord2f(1, 1); glVertex3f(player->trace[i].x, 2, -player->trace[i].y);
			pos = (fVec2){player->trace[i].x, -player->trace[i].y};
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void renderPlayer(int player) {

	glViewport(
		playerViewports[player].start.x * windowWidth,
		playerViewports[player].start.y * windowHeight,
		playerViewports[player].end.x * windowWidth,
		playerViewports[player].end.y * windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	CameraData *camera = getCameraData(player);
	glRotatef(camera->rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(camera->rotation.x, 0.0f, 1.0f, 0.0f);
	glRotatef(camera->rotation.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera->position.x, camera->position.y, camera->position.z);

	drawFloor();

	for(int c=0; c<countPlayers; c++) {
		glColor3f(getPlayerData(c)->color.x, getPlayerData(c)->color.y, getPlayerData(c)->color.z);
		drawTrace(c);
	}
	for(int c=0; c<countPlayers; c++) {
		glColor3f(getPlayerData(c)->color.x, getPlayerData(c)->color.y, getPlayerData(c)->color.z);
		drawBike(c, player);
	}
	glColor3f(1, 1, 1);
}
