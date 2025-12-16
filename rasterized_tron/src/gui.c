#include <stdio.h>
#include "../include/gui.h"
#include "../include/vector.h"
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
int selection = 0;
int intent = 0;
const char *settings[] = {
	"PLAY",
	"EXIT",
};

// --- Function declaration ---
void writeLine(const char *line, fVec2 start, fVec2 end);

// --- Entry point ---
void initGui(void) {
	initTexture(&font);
}

int getStatus() {
	return state;
}

void gui_update() {
	//printf("Update GUI\n");	
	if (selection > 1) selection = 0;
	if (selection < 0) selection = 1;

	if (intent ==-1) state = 2;
	if (intent == 1 && selection == 0) state = 1;
	if (intent == 1 && selection == 1) state = 2;
}

void gui_render() {
	//printf("Render GUI\n");
	///TODO implement button display
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	writeLine("TRON", (fVec2){-0.4f, 0.9f}, (fVec2){0.2f, 0.2f});

	for(int c=0; c<2; c++) {
		if (selection == c) glColor3f(0.5f, 1.0f, 0.2f);
		else glColor3f(1, 1, 1);
		writeLine(settings[c], (fVec2){-0.2f, 0.6f-(c*0.2f)}, (fVec2){0.1f, 0.1f});
	}
}

void gui_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;
	///TODO: if escape close game. Only start if set to game start
	if(key == GLFW_KEY_ESCAPE) {
		//state = 2;
		intent = -1;
		//glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if(key == GLFW_KEY_ENTER) {
		//state = 1;
		intent = 1;
	}

	//printf("Key: %c, %d\n", key, key);

	if(key == 265) selection -= 1;	// UP
	if(key == 264) selection += 1;	// DOWN
	//if(key == 263) pos_x -= 1;	// LEFT
	//if(key == 262) pos_x += 1;	// RIGHT
}

// --- Functions ---
void writeLine(const char *line, fVec2 start, fVec2 size) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font.texture);

	for(int c=0; line[c]!='\0'; c++) {
	int index = line[c] - 32;
	fVec2 pos = {(index % 10) / 10.f, (index / 10) / 10.f};

		///printf("%c: Tex(%f, %f), Pos(%f, %f)\n", line[c], pos.x, pos.y, start.x, start.y);
		
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f( pos.x, 		pos.y + 0.1f); glVertex2f( start.x,				start.y - size.y);
			glTexCoord2f( pos.x + 0.1f,	pos.y + 0.1f); glVertex2f( start.x + size.x, 	start.y - size.y);
			glTexCoord2f( pos.x, 		pos.y		); glVertex2f( start.x, 			start.y);
			glTexCoord2f( pos.x + 0.1f,	pos.y		); glVertex2f( start.x + size.x, 	start.y);
		glEnd();

		start.x += size.x;
		/// TODO: Start new line on overflow?
	}
	glDisable(GL_TEXTURE_2D);
}

