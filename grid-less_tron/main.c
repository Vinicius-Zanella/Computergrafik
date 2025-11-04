#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "vector.h"

// --- Config ---
#define WINDOW_SIZE 600
#define WALL_SIZE 100
#define WORLD_SIZE 500

// --- Globals
static iVec2 player_pos1 = { WORLD_SIZE / 4, WORLD_SIZE / 2};
static iVec2 player_pos2 = { WORLD_SIZE / 4 * 3, WORLD_SIZE / 2};

static char input1 = ' ', input2 = ' ';
static iVec2 player_wall1[WALL_SIZE] = {0};
static iVec2 player_wall2[WALL_SIZE] = {0};

static int index1 = 0, index2 = 0;

static float step_x = 2.0f / WORLD_SIZE;
static float step_y = 2.0f / WORLD_SIZE;

// --- Function declaration ---
void update(void);
void render(void);
void reset(void);
void addCorner(int player);
int isOutOfBounds(iVec2 pos);
//void drawQuad(int x, int y);
int collidedWithWall(iVec2 pos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);
	
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glPointSize(5.0f);
	glLineWidth(2.0f);

	reset();
	
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

	// - Draw Player 1 (Red) -
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(player_pos1.x * step_x - 1, player_pos1.y * step_y - 1);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f(player_pos1.x * step_x - 1, player_pos1.y * step_y - 1);
		for (int i=index1; i>=0; i--)
			glVertex2f(player_wall1[i].x * step_x - 1, player_wall1[i].y * step_y - 1);
	glEnd();

	// - Draw Player 2 -
	glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(player_pos2.x*step_x - 1, player_pos2.y*step_y - 1);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
		glVertex2f(player_pos2.x * step_x - 1, player_pos2.y * step_y - 1);
		for (int i=index2; i>=0; i--) 
			glVertex2f(player_wall2[i].x * step_x - 1, player_wall2[i].y * step_y - 1);
	glEnd();
}

void update(void) {
	// - Player 1 Movement -
	switch(input1) {
		case 'w': player_pos1.y += 1; break;
		case 'a': player_pos1.x -= 1; break;
		case 's': player_pos1.y -= 1; break;
		case 'd': player_pos1.x += 1; break;
	}
	
	// - Player 2 Movement -
	switch(input2) {
		case 'i': player_pos2.y += 1; break;
		case 'j': player_pos2.x -= 1; break;
		case 'k': player_pos2.y -= 1; break;
		case 'l': player_pos2.x += 1; break;
	}
	
	// - Collision & Bounds -
	if (isOutOfBounds(player_pos1)) reset();
	if (isOutOfBounds(player_pos2)) reset();
	if (collidedWithWall(player_pos1)) reset();
	if (collidedWithWall(player_pos2)) reset();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;	
	if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);

	// - Player 1 input -
	if (key == GLFW_KEY_W && input1 != 's' && input1 != 'w') { input1 = 'w'; addCorner(1); }
	if (key == GLFW_KEY_A && input1 != 'd' && input1 != 'a') { input1 = 'a'; addCorner(1); }
	if (key == GLFW_KEY_S && input1 != 'w' && input1 != 's') { input1 = 's'; addCorner(1); }
	if (key == GLFW_KEY_D && input1 != 'a' && input1 != 'd') { input1 = 'd'; addCorner(1); }
	
	// - Player 2 input -
	if (key == GLFW_KEY_I && input2 != 'k' && input2 != 'i') { input2 = 'i'; addCorner(2); }
	if (key == GLFW_KEY_J && input2 != 'l' && input2 != 'j') { input2 = 'j'; addCorner(2); }
	if (key == GLFW_KEY_K && input2 != 'i' && input2 != 'k') { input2 = 'k'; addCorner(2); }
	if (key == GLFW_KEY_L && input2 != 'j' && input2 != 'l') { input2 = 'l'; addCorner(2); }
}

void addCorner(int player) {
	iVec2 *wall = (player == 1) ? player_wall1 : player_wall2;
	iVec2 *pos  = (player == 1) ? &player_pos1 : &player_pos2;
	int *index  = (player == 1) ? &index1 : &index2;

	if (*index >= WALL_SIZE-1) {
		printf("Reached maximum wall for Player%d\n", player);
		reset();
		return;
	}
	
	(*index)++;
	wall[*index] = *pos;
}

int isOutOfBounds(iVec2 pos) {
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

int collidedWithWall(iVec2 pos) {
	for (int p=1; p<=2; p++) {
		iVec2 *wall = (p == 1) ? player_wall1 : player_wall2;
		int index = (p == 1) ? index1 : index2;
		iVec2 *currentPos = (p == 1) ? &player_pos1 : &player_pos2;

		for (int c=0; c<index; c++) {
			// - Horizontal -
			if (wall[c].y == pos.y && wall[c+1].y == pos.y) {
				if (fmin(wall[c].x, wall[c+1].x) < pos.x && pos.x < fmax(wall[c].x, wall[c+1].x))
					return 1;
			}
			// - Vertical -
			if (wall[c].x == pos.x && pos.x == wall[c+1].x) {
				if (fmin(wall[c].y, wall[c+1].y) < pos.y && pos.y < fmax(wall[c].y, wall[c+1].y))
					return 1;
			}
		}

		// Active segment
		if (wall[index].y == currentPos->y && pos.y == currentPos->y) {
			if (fmin(wall[index].x, currentPos->x) < pos.x && pos.x < fmax(wall[index].x, currentPos->x))
				return 1;
		}
		if (wall[index].x == currentPos->x && pos.x == currentPos->x) {
			if (fmin(wall[index].y, currentPos->y) < pos.y && pos.y < fmax(wall[index].y, currentPos->y))
				return 1;
		}
	}
	return 0;
}

void reset(void) {
	memset(player_wall1, 0, sizeof(player_wall1));
	memset(player_wall2, 0, sizeof(player_wall2));
	
	player_pos1 = (iVec2) { WORLD_SIZE / 4, WORLD_SIZE / 2 };
	player_pos2 = (iVec2) { WORLD_SIZE / 4 * 3, WORLD_SIZE / 2 };
	
	player_wall1[0] = player_pos1;
	player_wall2[0] = player_pos2;

	index1 = 0;
	index2 = 0;
	
	input1 = 'd';
	input2 = 'j';
}
