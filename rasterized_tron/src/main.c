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
static int menu(void);
static int game(void);

// --- Entry Point ---
int main(void) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	printf("Initiating Mneu...\n");
	menu();

	if (getStatus() == 1) {
		printf("Entering game...\n");
		return game();
	}

	return 0;
}

static int menu(void) {
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
	glfwSwapInterval(1);	// VSync

	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// - Callback -
	glfwSetFramebufferSizeCallback(window, gui_resize);
	glfwSetKeyCallback(window, gui_keyCallback);

	initGui();

	// -- Core Loop --
	while(!glfwWindowShouldClose(window)) {
		gui_update();
		gui_render();
		
		glfwSwapBuffers(window);		
		glfwPollEvents();
		
		if(getStatus() > 0)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	// -- Clean up --
	glfwDestroyWindow(window);
	return 0;
}

static int game(void) {	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rasterized Tron", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);	// VSync

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

	// - Callback -
	glfwSetFramebufferSizeCallback(window, render_resize);
	glfwSetKeyCallback(window, keyCallback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// - 3D Config -
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const double near = 0.01, far = 1000.0, fov = 100.0;
	const double aspect = WINDOW_WIDTH / WINDOW_HEIGHT;

	const double top = (fov * 0.5 * M_PI / 180.0) * near;
	const double bottom = -top, right = top * aspect, left = -right;

	glFrustum(left, right, bottom, top, near, far);

	// Window alignment based on player count
	struct displayArea viewports[4];
	if (PLAYER_COUNT == 1) {
		viewports[0] = displayPositions[FULL];
	}
	if (PLAYER_COUNT == 2) {
		viewports[0] = displayPositions[WEST];
		viewports[1] = displayPositions[EAST];
	}
	if (PLAYER_COUNT > 2) {
		viewports[0] = displayPositions[NORTHWEST];
		viewports[1] = displayPositions[NORTHEAST];
		viewports[2] = displayPositions[SOUTHWEST];
		viewports[3] = displayPositions[SOUTHEAST];
	}
	
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
