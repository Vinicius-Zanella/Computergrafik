#include <stdio.h>
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
	} else {
		// - Player 1 input -
		Input i = getInput(1);
		char *input = &getPlayerData(1)->input;
		if (key == (int)i.Upwa && *input != i.Down && *input != i.Upwa) { *input = i.Upwa; addCorner(1); } else
		if (key == (int)i.Left && *input != i.Rigt && *input != i.Left) { *input = i.Left; addCorner(1); } else
		if (key == (int)i.Down && *input != i.Upwa && *input != i.Down) { *input = i.Down; addCorner(1); } else
		if (key == (int)i.Rigt && *input != i.Left && *input != i.Rigt) { *input = i.Rigt; addCorner(1); }
	
		// - Player 2 input -
		i = getInput(2);
		input = &getPlayerData(2)->input;
		if (key == (int)i.Upwa && *input != i.Down && *input != i.Upwa) { *input = i.Upwa; addCorner(2); } else
		if (key == (int)i.Left && *input != i.Rigt && *input != i.Left) { *input = i.Left; addCorner(2); } else
		if (key == (int)i.Down && *input != i.Upwa && *input != i.Down) { *input = i.Down; addCorner(2); } else
		if (key == (int)i.Rigt && *input != i.Left && *input != i.Rigt) { *input = i.Rigt; addCorner(2); }
	}
}
