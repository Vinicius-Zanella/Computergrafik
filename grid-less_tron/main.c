#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "vector.h"

// --- Config ---
#define WINDOW_SIZE 600
#define FRAMERATE 10.0
#define WALL_SIZE 100

// --- Globals
static const int WORLD_SIZE = 500;
static iVec2 player_pos1={WORLD_SIZE/4, WORLD_SIZE/2}, player_pos2={WORLD_SIZE/4*3, WORLD_SIZE/2};
static char input1=' ', input2=' ';
static iVec2 player_wall1[WALL_SIZE] = {0}, player_wall2[WALL_SIZE] = {0};
static int index1=0, index2=0;
static float step_x = 2.0f / WORLD_SIZE;
static float step_y = 2.0f / WORLD_SIZE;

// --- Function declaration ---
void update();
void render(void);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void reset();
void drawQuad(int x, int y);
int isOutOfBounds(iVec2 pos);
int collidedWithWall(iVec2 pos);
void addCorner(int player);

// --- Entry Point ---
int main() {
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

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
	glfwSwapInterval(1);
	
	glfwSetKeyCallback(window, key_callback);

	reset(window);
	
	glPointSize(5.f);
	glLineWidth(2.f);

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

// --- Functions ---
void render(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw one
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
		glVertex2f(player_pos1.x * step_x - 1, player_pos1.y * step_y - 1);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2f(player_pos1.x * step_x - 1, player_pos1.y * step_y - 1);
		for (int i=index1; i>=0; i--) {
			glVertex2f(player_wall1[i].x * step_x - 1, player_wall1[i].y * step_y - 1);
		}
	glEnd();

	// Draw two
	glBegin(GL_POINTS);
		glColor3f(0, 1, 0);
		glVertex2f(player_pos2.x*step_x - 1, player_pos2.y*step_y - 1);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2f(player_pos2.x * step_x - 1, player_pos2.y * step_y - 1);
		for (int i=index2; i>=0; i--) {
			glVertex2f(player_wall2[i].x * step_x - 1, player_wall2[i].y * step_y - 1);
		}
	glEnd();
}

void update() {
	// Player 1 Movement
	switch(input1) {
		case 'w': player_pos1.y += 1; break;
		case 'a': player_pos1.x -= 1; break;
		case 's': player_pos1.y -= 1; break;
		case 'd': player_pos1.x += 1; break;
	}
	//player_wall1[index1] = player_pos1;
	
	// Player 2 Movement
	switch(input2) {
		case 'i': player_pos2.y += 1; break;
		case 'j': player_pos2.x -= 1; break;
		case 'k': player_pos2.y -= 1; break;
		case 'l': player_pos2.x += 1; break;
	}
	//player_wall2[index2] = player_pos2;
	
	// Collision & Bounds
	if (isOutOfBounds(player_pos1)) reset();
	//if (player_pos1.x[player_pos1.y != 0) reset(window);
	
	if (isOutOfBounds(player_pos2)) reset();
	//if (grid[player2_pos[0]][player2_pos[1]] != 0) reset(window);

	if (collidedWithWall(player_pos1)) reset();
	if (collidedWithWall(player_pos2)) reset();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;
	
	if(key == GLFW_KEY_ESCAPE) 
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// Player 1 input
	if (key == GLFW_KEY_W && input1 != 's' && input1 != 'w') {
		input1 = 'w';
		addCorner(1);
	}
	if (key == GLFW_KEY_A && input1 != 'd' && input1 != 'a') {
		input1 = 'a';
		addCorner(1);
	}
	if (key == GLFW_KEY_S && input1 != 'w' && input1 != 's') {
		input1 = 's';
		addCorner(1);
	}
	if (key == GLFW_KEY_D && input1 != 'a' && input1 != 'd') {
		input1 = 'd';
		addCorner(1);
	}
	
	// Player 2 input
	if (key == GLFW_KEY_I && input2 != 'k') {
		input2 = 'i';
		addCorner(2);
	}
	if (key == GLFW_KEY_J && input2 != 'l') {
		input2 = 'j';
		addCorner(2);
	}
	if (key == GLFW_KEY_K && input2 != 'i') {
		input2 = 'k';
		addCorner(2);
	}
	if (key == GLFW_KEY_L && input2 != 'j') {
		input2 = 'l';
		addCorner(2);
	}
}

void addCorner(int player) {
	if (player == 1) {
		if (index1 >= WALL_SIZE-1) {
			printf("Reached maximum wall for Player%d\n", player);
			reset();
			return;
		}
		index1++;
		player_wall1[index1] = player_pos1;
	}
	else if (player == 2) {
		if (index2 >= WALL_SIZE-1) {
			printf("Reached maximum wall for Player%d\n", player);
			reset();
			return;
		}
		index2++;
		player_wall2[index2] = player_pos2;
	}
}

int isOutOfBounds(iVec2 pos) {
	///TODO: make world not necessarily squared (WORLD_SIZE.x, WORLD_SIZE.y)
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

int collidedWithWall(iVec2 pos) {
	///TODO Catch newest line of player
	for (int i=0; i<index1; i++) {
		// Player 1 horizontal
		if (player_wall1[i].y == pos.y) {
			if (player_wall1[i+1].y == pos.y) {///TODO Figure out if out of bounds can happen
				if (fmin(player_wall1[i].x, player_wall1[i+1].x) < pos.x && pos.x < fmax(player_wall1[i].x, player_wall1[i+1].x)) return 1;
			}
		}
		// Player 1 vertical
		if (player_wall1[i].x == pos.x) {
			if (player_wall1[i+1].x == pos.x) {
				if (fmin(player_wall1[i].y, player_wall1[i+1].y) < pos.y && pos.y < fmax(player_wall1[i].y, player_wall1[i+1].y)) return 1;
			}
		}
	}
	// Player 1 active segment
	if (player_wall1[index1].y == pos.y && pos.y == player_pos1.y) {
		if (fmin(player_wall1[index1].x, player_pos1.x) < pos.x && pos.x < fmax(player_wall1[index1].x, player_pos1.x)) return 1;
	}
	if (player_wall1[index1].x == pos.x && pos.x == player_pos1.x) {
		if (fmin(player_wall1[index1].y, player_pos1.y) < pos.y && pos.y < fmax(player_wall1[index1].y, player_pos1.y)) return 1;
	}
	
	for (int i=0; i<index2; i++) {
		// Player 2 Horizontal
		if (player_wall2[i].y == pos.y) {
			if (player_wall2[i+1].y == pos.y) {///TODO Figure out if out of bounds can happen
				if (fmin(player_wall2[i].x, player_wall2[i+1].x) < pos.x && pos.x < fmax(player_wall2[i].x, player_wall2[i+1].x)) return 1;
			}
		}
		// Player 2 Vertical
		if (player_wall2[i].x == pos.x) {
			if (player_wall2[i+1].x == pos.x) {
				if (fmin(player_wall2[i].y, player_wall2[i+1].y) < pos.y && pos.y < fmax(player_wall2[i].y, player_wall2[i+1].y)) return 1;
		}
	}
	// Player 2 active segment
	if (player_wall2[index2].y == pos.y && pos.y == player_pos2.y) {
		if (fmin(player_wall2[index2].x, player_pos2.x) < pos.x && pos.x < fmax(player_wall2[index2].x, player_pos2.x)) return 1;
	}
	if (player_wall2[index2].x == pos.x && pos.x == player_pos2.x) {
		if (fmin(player_wall2[index2].y, player_pos2.y) < pos.y && pos.y < fmax(player_wall2[index2].y, player_pos2.y)) return 1;
	}
	return 0;
	}
}

void reset() {
	memset(player_wall1, 0, sizeof(player_wall1));
	memset(player_wall2, 0, sizeof(player_wall2));
	
	player_pos1.x = WORLD_SIZE / 4;
	player_pos1.y = WORLD_SIZE / 2;
	player_pos2.x = WORLD_SIZE / 4 * 3;
	player_pos2.y = WORLD_SIZE / 2;
	
	player_wall1[0] = player_pos1;
	player_wall2[0] = player_pos2;

	index1 = 0;
	index2 = 0;
	
	input1 = 'a';
	input2 = 'l';
}
