#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/input.h"

// --- Global ---
int playercount = 0;

// --- Function Declaration ---
void turn(int p);


/// --- Entry Point ---
void initInput(int count) {
	playercount = count;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;
	if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

	//printf("Key: %c, Nr: %d\n", key, key);

	if (key == ' ') {
		spectator = (spectator == 1) ? 0 : 1;
		printf("Spectator: %d\n", spectator);
	} 

	if (spectator) {
		// - Camera movement -
		Input i = getInput(0);
		int *input = &getCameraData(0)->input;
		if (key == (int)i.Upwa) { *input = (*input == (int)i.Upwa) ? ' ' : i.Upwa; } else
		if (key == (int)i.Left) { *input = (*input == (int)i.Left) ? ' ' : i.Left; } else
		if (key == (int)i.Down) { *input = (*input == (int)i.Down) ? ' ' : i.Down; } else
		if (key == (int)i.Rigt) { *input = (*input == (int)i.Rigt) ? ' ' : i.Rigt; } else
		if (key == (int)i.Qsud) { *input = (*input == (int)i.Qsud) ? ' ' : i.Qsud; } else
		if (key == (int)i.Esud) { *input = (*input == (int)i.Esud) ? ' ' : i.Esud; }
		i = getInput(1);
		if (key == (int)i.Upwa) { *input = (*input == (int)i.Upwa) ? ' ' : i.Upwa; } else
		if (key == (int)i.Left) { *input = (*input == (int)i.Left) ? ' ' : i.Left; } else
		if (key == (int)i.Down) { *input = (*input == (int)i.Down) ? ' ' : i.Down; } else
		if (key == (int)i.Rigt) { *input = (*input == (int)i.Rigt) ? ' ' : i.Rigt; } else
		if (key == (int)i.Qsud) { *input = (*input == (int)i.Qsud) ? ' ' : i.Qsud; } else
		if (key == (int)i.Esud) { *input = (*input == (int)i.Esud) ? ' ' : i.Esud; }

		if (key == 'C') {
			CameraData *camera = getCameraData(1);
			printf("Camera: (%.1f, %.1f, %.1f)(%.1f, %.1f, %.1f)\n", camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y, camera->rotation.z);
		}
	} else {
		// - Player input -
		for (int p=0; p<playercount; p++) {
			Input i = getInput(p);
			Direction *direction = &getPlayerData(p)->direction;
			//printf("Pl pos: (%d, %d); ter rot: (%.0f)\n", getPlayerData(1)->position.x, getPlayerData(1)->position.y, getCameraData(1)->rotation.x);
			if (key == (int)i.Left) {
				*direction = *direction - 1;
				if(*direction > 3) {
					*direction = 3;	// I am using an enum, so it is unsigned. -1 results in a negative overflow
					getCameraData(p)->rotation.x = 360;	// I have to TP the camera by 360° So that the camera stays in the confids between -90 and 360
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
	}
}

void turn(int p) {
	CameraData *camera = getCameraData(p);
	Direction *direction = &getPlayerData(p)->direction;
	addCorner(p);
	camera->targetRotation.x = (*direction) / 4.0f * 360.0f;
	camera->targetPosition = (fVec3){-getPlayerData(p)->position.x + sin(camera->targetRotation.x / 180 * M_PI) * 25, -10, getPlayerData(p)->position.y - cos(camera->targetRotation.x / 180 * M_PI) * 25};	
	//printf("After turn: player(%d, %d), camera(%.0f, %.0f, %.0f)\n", getPlayerData(1)->position.x, getPlayerData(1)->position.y, camera->targetPosition.x, camera->targetPosition.y, camera->targetPosition.z);
	//printf("Direction: %.0f, delta = %.1f\n", camera->targetRotation.x, (camera->targetRotation.x - camera->rotation.x));
}
				
