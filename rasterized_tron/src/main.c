#include <stdio.h>
#include <math.h>
#include "../include/gui.h"
#include "../include/update.h"
#include "../include/input.h"
#include "../include/render.h"

// --- Config ---
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define PLAYER_COUNT 2
// --- Declaration ---
int menu();
int game();

// --- Entry Point ---
int main() {
	// - Set version -
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	printf("Initiating Mneu...\n");
	menu();

	if (getStatus() == 1) {
		printf("Entering game...\n");
		game();
	}
}

int menu() {
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(720, 720,  "Tron Menu", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);	// Background Colour
	glfwSwapInterval(1);	// VSync

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// - Callback -
	glfwSetFramebufferSizeCallback(window, gui_resize);
	glfwSetKeyCallback(window, gui_keyCallback);

	///TODO remove
	glPointSize(20.0f);

	initGui(WINDOW_WIDTH, WINDOW_HEIGHT);

	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);	

	// -- Core Loop --
	while(!glfwWindowShouldClose(window)) {
		gui_update();
		gui_render();
		
		glfwSwapBuffers(window);		
		glfwPollEvents();
		if(getStatus() > 0) glfwSetWindowShouldClose(window, GLFW_TRUE);	// enter game
	}

	// -- Clean up --
	glfwDestroyWindow(window);
	return 0;
}


int game() {	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rasterized Tron", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);	// Background Colour
	glfwSwapInterval(1);	// VSync

	// - 3D Config -
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double near = 0.01, far = 1000.0, fov = 90.0;
	double aspect = WINDOW_WIDTH / WINDOW_HEIGHT;

	double top = (fov * 0.5 * M_PI / 180.0) * near;
	double bottom = -top, right = top * aspect, left = -right;

	glFrustum(left, right, bottom, top, near, far);

	// - Callback -
	glfwSetFramebufferSizeCallback(window, render_resize);
	glfwSetKeyCallback(window, keyCallback);
	
	///TODO remove
	glPointSize(20.0f);
	glLineWidth(20.0f);

	/// TODO: calculate displayArea struct

	struct displayArea viewports[] = {
		displayPositions[EAST],
		displayPositions[WEST],
	};
	initRender(PLAYER_COUNT, WINDOW_WIDTH, WINDOW_HEIGHT, viewports);
	initInput(PLAYER_COUNT);
	initGame(PLAYER_COUNT);

	float lastTime = 0.0f;
	
	// -- Core Loop --
	while(!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;
	
		game_update(dt);
		game_render();
		
		glfwSwapBuffers(window);		
		glfwPollEvents();
	}

	// -- Clean up --
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
