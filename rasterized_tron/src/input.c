#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/input.h"

// --- Global ---
int playercount;

// --- Function Declaration ---
static void playerInput(int p, int key);
static void turn(int p);

// --- Entry Point ---
void initInput(int count) {
	playercount = count;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)scancode;		// Silence compiler
	(void)mods;

	if (action != GLFW_PRESS) return;
	
	if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

	key = key % 255;
	
	// - Player input -
	for (int p=0; p<playercount; p++) {
			playerInput(p, key);
	}
}


// -- Functions --

static void playerInput(int p, int key) {
	Input i = getInput(p);
	Direction *direction = &getPlayerData(p)->direction;
	
	if (key == (int)i.Left) {
		*direction = *direction - 1;
		if(*direction > 3) {
			*direction = 3;	// enum is an unsigned int
			getCameraData(p)->rotation.x = 360;
		}
		turn(p);
	} else if (key == (int)i.Rigt) {
		*direction = *direction + 1;
		if(*direction > 3) {
			*direction = 0;
			getCameraData(p)->rotation.x = -90;
		}
		turn(p);
	}
}

static void turn(int p) {
	CameraData *camera = getCameraData(p);
	PlayerData *player = getPlayerData(p);
	
	addCorner(p);
	camera->targetRotation.x = (player->direction) / 4.0f * 360.0f;
	float angle = camera->targetRotation.x / 180.f * M_PI;
	camera->targetPosition =
		(fVec3){-player->position.x +  sin(angle) * 25,
		-10,
		player->position.y - cos(angle) * 25
		};
}
