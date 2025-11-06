#include <stdio.h>
#include <math.h>
#include "../include/update.h"
#include "../include/input.h"
#include "../include/render.h"

// --- Config ---
#define WINDOW_SIZE 600

// --- Entry Point ---
int main() {
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	// - Set version -
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Rasterized Tron", NULL, NULL);
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
	double aspect = WINDOW_SIZE / WINDOW_SIZE;

	double top = (fov * 0.5 * M_PI / 180.0) * near;
	double bottom = -top, right = top * aspect, left = -right;

	glFrustum(left, right, bottom, top, near, far);

	// - Callback -
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, keyCallback);
	
	///TODO remove
	glPointSize(20.0f);
	glLineWidth(20.0f);

	initRender();
	initGame();

	float lastTime = 0.0f;
	
	// -- Core Loop --
	while(!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;
	
		update(dt);
		render();
		
		glfwSwapBuffers(window);		
		glfwPollEvents();
	}

	// -- Clean up --
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
