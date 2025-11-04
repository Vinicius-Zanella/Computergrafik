#include <GL/gl.h>
#include "../include/world.h"
#include "../include/render.h"

// -- Global --
static fVec2 step = {2.0f / WORLD_SIZE, 2.0f / WORLD_SIZE};

// -- Function declaration --

// --- Entry Point ---
void render(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	///TODO loop players
	// - Draw Player 1 (Red) -
	PlayerData *player = getPlayerData(1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(player->position.x * step.x - 1, player->position.y * step.y - 1);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2f(player->position.x * step.x - 1, player->position.y * step.y - 1);
		for (int i=player->index; i>=0; i--)
			glVertex2f(player->trace[i].x * step.x - 1, player->trace[i].y * step.y - 1);
	glEnd();

	// - Draw Player 2 -
	player = getPlayerData(2);
	glBegin(GL_POINTS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(player->position.x*step.x - 1, player->position.y*step.y - 1);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
		glVertex2f(player->position.x * step.x - 1, player->position.y * step.y - 1);
		for (int i=player->index; i>=0; i--) 
			glVertex2f(player->trace[i].x * step.x - 1, player->trace[i].y * step.y - 1);
	glEnd();
}
