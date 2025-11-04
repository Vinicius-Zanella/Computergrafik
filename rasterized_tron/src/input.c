#include "../include/world.h"
#include "../include/input.h"

///TODO make the input in itself consistent, but make it custom mappable from outside
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;	
	if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

	// - Player 1 input -
	char *input = &getPlayerData(1)->input;
	if (key == GLFW_KEY_W && *input != 'S' && *input != 'W') { *input = 'W'; addCorner(1); }
	if (key == GLFW_KEY_A && *input != 'D' && *input != 'A') { *input = 'A'; addCorner(1); }
	if (key == GLFW_KEY_S && *input != 'W' && *input != 'S') { *input = 'S'; addCorner(1); }
	if (key == GLFW_KEY_D && *input != 'A' && *input != 'D') { *input = 'D'; addCorner(1); }
	
	// - Player 2 input -
	input = &getPlayerData(2)->input;
	if (key == GLFW_KEY_I && *input != 'K' && *input != 'I') { *input = 'I'; addCorner(2); }
	if (key == GLFW_KEY_J && *input != 'L' && *input != 'J') { *input = 'J'; addCorner(2); }
	if (key == GLFW_KEY_K && *input != 'I' && *input != 'K') { *input = 'K'; addCorner(2); }
	if (key == GLFW_KEY_L && *input != 'J' && *input != 'L') { *input = 'L'; addCorner(2); }
}
