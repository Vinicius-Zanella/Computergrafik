#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

// --- Config ---
#define MAP_X 128
#define MAP_Y 128
#define WINDOW_SIZE 600
#define FRAMERATE 10.0

// --- Globals
static int grid[MAP_X][MAP_Y];
static int player1_pos[2], player2_pos[2];
static char input1, input2;
static float cell_x = 2.0f / MAP_X;
static float cell_y = 2.0f / MAP_Y;

// --- Function declaration ---
void update(GLFWwindow* window);
void render(void);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void reset(GLFWwindow* window);
void drawQuad(int x, int y);
int isOutOfBounds(int x, int y);

// --- Entry Point ---
int main() {
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	// init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Simple Tron", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glfwSetKeyCallback(window, key_callback);

	reset(window);
	
	double lastFrame = 0.0;
	const double frameDelay = 1.0 / FRAMERATE;

	// Core Loop
	while(!glfwWindowShouldClose(window)) {
		double now = glfwGetTime();
		
		glfwPollEvents();

		if (now - lastFrame >= frameDelay) {
			render();
			glfwSwapBuffers(window);
			render();
			update(window);
			lastFrame = now;
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// --- Functions ---
void drawQuad(int x, int y) {
	float min_x =-1.0f + x * cell_x;
	float min_y =-1.0f + y * cell_y;
	float max_x = min_x + cell_x;
	float max_y = min_y + cell_y;
	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(min_x, min_y);
		glVertex2f(min_x, max_y);
		glVertex2f(max_x, min_y);
		glVertex2f(max_x, max_y);
	glEnd();
}

void render(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	drawQuad(player1_pos[0], player1_pos[1]);
	
	glColor3f(0.0f, 0.0f, 1.0f);
	drawQuad(player2_pos[0], player2_pos[1]);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;
	
	if(key == GLFW_KEY_ESCAPE) 
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Player 1 input
	if (key == GLFW_KEY_W && input1 != 's') input1 = 'w';
	if (key == GLFW_KEY_A && input1 != 'd') input1 = 'a';
	if (key == GLFW_KEY_S && input1 != 'w') input1 = 's';
	if (key == GLFW_KEY_D && input1 != 'a') input1 = 'd';
	
	// Player 2 input
	if (key == GLFW_KEY_I && input2 != 'k') input2 = 'i';
	if (key == GLFW_KEY_J && input2 != 'l') input2 = 'j';
	if (key == GLFW_KEY_K && input2 != 'i') input2 = 'k';
	if (key == GLFW_KEY_L && input2 != 'j') input2 = 'l';
}

int isOutOfBounds(int x, int y) {
	return (x < 0 || y < 0 || x >= MAP_X || y >= MAP_Y);
}

void update(GLFWwindow* window) {
	// Player 1 Movement
	switch(input1) {
		case 'w': player1_pos[1]++; break;
		case 'a': player1_pos[0]--; break;
		case 's': player1_pos[1]--; break;
		case 'd': player1_pos[0]++; break;
	}

	// Player 2 Movement
	switch(input2) {
		case 'i': player2_pos[1]++; break;
		case 'j': player2_pos[0]--; break;
		case 'k': player2_pos[1]--; break;
		case 'l': player2_pos[0]++; break;
	}

	// Collision & Bounds
	if (isOutOfBounds(player1_pos[0], player1_pos[1])) reset(window);
	if (grid[player1_pos[0]][player1_pos[1]] != 0) reset(window);

	if (isOutOfBounds(player2_pos[0], player2_pos[1])) reset(window);
	if (grid[player2_pos[0]][player2_pos[1]] != 0) reset(window);

	grid[player1_pos[0]][player1_pos[1]] = 1;
	grid[player2_pos[0]][player2_pos[1]] = 1;
}

void reset(GLFWwindow* window) {
	memset(grid, 0, sizeof(grid));
	
	player1_pos[0] = MAP_X/2;
	player1_pos[1] = MAP_Y/2;
	player2_pos[0] = MAP_X/2 + 1;
	player2_pos[1] = MAP_Y/2;

	input1 = 'a';
	input2 = 'l';
	
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glClear(GL_COLOR_BUFFER_BIT);

}
