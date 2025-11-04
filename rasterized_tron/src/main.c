#include <stdio.h>
#include "../include/world.h"
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Simple Tron", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);
	
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glPointSize(5.0f);
	glLineWidth(2.0f);

	resetWorld();
	
	// -- Core Loop --
	while(!glfwWindowShouldClose(window)) {
		update();
		render();
		
		glfwSwapBuffers(window);		
		glfwPollEvents();
	}

	// -- Clean up --
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
