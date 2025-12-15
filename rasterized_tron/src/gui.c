#include <stdio.h>
#include "../include/gui.h"
#include "../include/texture.h"

///TODO temp
int pos_x = 0;
int pos_y = 0;
int state = 0; // 0=menu, 1=game, 2=exit

// --- Global ---
//static TexStruct font = { 0, .filename = "assets/Tron_font-white.svg" };
static TexStruct font = { 0, .filename = "assets/Tron_font-white.png" };

// --- Config ---
///TODO Settings and variables

// --- Layout ---
///TODO Buttons

// --- Function declaration ---


// --- Entry point ---
void initGui(void) {
	initTexture(&font);
}

int getStatus() {
	return state;
}

void gui_update() {
	//printf("Update GUI\n");
	///TODO implement button logic and settings
	printf("(%d, %d)\n", pos_x, pos_y);
}

void gui_render() {
	//printf("Render GUI\n");
	///TODO implement button display
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font.texture);
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f( 1,-1); glVertex2f(-0.9f,-0.9f);
		glTexCoord2f( 1, 1); glVertex2f( 0.9f,-0.9f);
		glTexCoord2f(-1,-1); glVertex2f(-0.9f, 0.9f);
		glTexCoord2f(-1, 1); glVertex2f( 0.9f, 0.9f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void gui_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;
	///TODO: if escape close game. Only start if set to game start
	if(key == GLFW_KEY_ESCAPE) {
		state = 2;
		//glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if(key == GLFW_KEY_ENTER) {
		state = 1;
	}

	//printf("Key: %c, %d\n", key, key);

	if(key == 265) pos_y += 1;	// UP
	if(key == 263) pos_x -= 1;	// LEFT
	if(key == 264) pos_y -= 1;	// DOWN
	if(key == 262) pos_x += 1;	// RIGHT
}

