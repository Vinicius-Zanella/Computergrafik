#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/input.h"

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
		Input i = getInput(1);
		int *input = &getCameraData(1)->input;
		if (key == (int)i.Upwa) { *input = (*input == (int)i.Upwa) ? ' ' : i.Upwa; } else
		if (key == (int)i.Left) { *input = (*input == (int)i.Left) ? ' ' : i.Left; } else
		if (key == (int)i.Down) { *input = (*input == (int)i.Down) ? ' ' : i.Down; } else
		if (key == (int)i.Rigt) { *input = (*input == (int)i.Rigt) ? ' ' : i.Rigt; } else
		if (key == (int)i.Qsud) { *input = (*input == (int)i.Qsud) ? ' ' : i.Qsud; } else
		if (key == (int)i.Esud) { *input = (*input == (int)i.Esud) ? ' ' : i.Esud; }
		i = getInput(2);
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
		// - Player 1 input -
		for (int p=1; p<=2; p++) {
			Input i = getInput(p);
			Direction *direction = &getPlayerData(p)->direction;
			CameraData *camera = getCameraData(1);
			if (key == (int)i.Left) {	///TODO The world is mirrored (up-down)
				*direction = (*direction - 1) % 4;
				addCorner(p);	///TODO combine redundant left & right code
				camera->rotation.x = (*direction) / 4.0f * 360.0f;
				camera->position = (fVec3){-getPlayerData(1)->position.x + sin(camera->rotation.x/180*M_PI) * 10, -10, getPlayerData(1)->position.y - cos(camera->rotation.x/180*M_PI) * 10};
			} else if (key == (int)i.Rigt) {
				*direction = (*direction + 1) % 4;
				addCorner(p);
				camera->rotation.x = (*direction) / 4.0f * 360.0f;
				camera->position = (fVec3){-getPlayerData(1)->position.x + sin(camera->rotation.x/180*M_PI) * 10, -10, getPlayerData(1)->position.y - cos(camera->rotation.x/180*M_PI) * 10};
			}
			
			/*
			if (key == (int)i.Upwa && *input != i.Down && *input != i.Upwa) { *input = i.Upwa; addCorner(p); } else
			if (key == (int)i.Left && *input != i.Rigt && *input != i.Left) { *input = i.Left; addCorner(p); } else
			if (key == (int)i.Down && *input != i.Upwa && *input != i.Down) { *input = i.Down; addCorner(p); } else
			if (key == (int)i.Rigt && *input != i.Left && *input != i.Rigt) { *input = i.Rigt; addCorner(p); }
			*/	
		}
	}
}
