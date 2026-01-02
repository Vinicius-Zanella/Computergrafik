#include <stdio.h>
#include "../include/gui.h"
#include "../include/vector.h"
#include "../include/texture.h"

int state = 0; // 0=menu, 1=game, 2=exit
enum state { MENU, GAME, EXIT };
enum intent { ESCAPE = -1, ENTER = 1 };

// --- Global ---
static TexStruct font = { 0, .filename = "assets/Tron_font-white.png" };

// --- Layout ---
static int selection = 0;
static int intent = 0;
static const char *settings[] = {
	"PLAY",
	"EXIT",
};

// --- Function declaration ---
static void writeLine(const char *line, fVec2 start, fVec2 end);

// --- Entry point ---
void initGui() {
	initTexture(&font);
}

int getStatus() {
	return state;
}

void gui_resize(GLFWwindow *window, int width, int height) {
	(void)window;	// Shut up the compiler
	glViewport(0, 0, width, height);
}

void gui_update(void) {
	if (selection > 1) selection = 0;
	if (selection < 0) selection = 1;

	if (intent == ESCAPE) state = EXIT;
	else if (intent == ENTER) {
		if (selection == 0) state = GAME;
		if (selection == 1) state = EXIT;
	}
}

void gui_render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	
	writeLine("TRON",
		(fVec2){-0.4f, 0.9f},
		(fVec2){0.2f, 0.2f});

	for(int c=0; c<2; c++) {
		if (selection == c)
			glColor3f(0.5f, 1.0f, 0.2f);
		else
			glColor3f(1, 1, 1);

		writeLine(settings[c],
			(fVec2){-0.2f, 0.6f-(c*0.2f)},
			(fVec2){0.1f, 0.1f});
	}
}

void gui_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	(void)window;
	(void)scancode;
	(void)mods;	// Shut up the compiler
	
	if (action != GLFW_PRESS) return;

	switch (key) {
		case GLFW_KEY_ESCAPE:
			intent = ESCAPE;
			break;
		case GLFW_KEY_ENTER:
			intent = ENTER;
			break;
		case 265:	// UP
			selection -= 1;
			break;
		case 264:	// DOWN
			selection += 1;
			break;
	}
}

// --- Text rendering ---
static void writeLine(const char *line, fVec2 start, fVec2 size) {
	if (!line || !font.texture) return;	// Negative space programming

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font.texture);

	for(int c=0; line[c]!='\0'; c++) {
		int index = line[c] - 32;
		fVec2 pos = {
			(index % 10) / 10.f,
			(index / 10) / 10.f
		};
		
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f( pos.x, 		pos.y + 0.1f); glVertex2f( start.x,				start.y - size.y);
			glTexCoord2f( pos.x + 0.1f,	pos.y + 0.1f); glVertex2f( start.x + size.x, 	start.y - size.y);
			glTexCoord2f( pos.x, 		pos.y		); glVertex2f( start.x, 			start.y);
			glTexCoord2f( pos.x + 0.1f,	pos.y		); glVertex2f( start.x + size.x, 	start.y);
		glEnd();

		start.x += size.x;
	}
	glDisable(GL_TEXTURE_2D);
}

