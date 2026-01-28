#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/world.h"

#define MAX_PLAYERS  4

int spectator = 0;

// - Custom input handling -
static const Input input[MAX_PLAYERS] = {
	{'W', 'A', 'S', 'D', 'Q', 'E'},
	{'I', 'J', 'K', 'L', 'U', 'O'},
	{10, 8, 9, 7, 11, 12},
	{'G', 'V', 'B', 'N', 'F', 'H'},
};

Input getInput(int i) {
	return input[i];
}


// - Player stuff -
static PlayerData players[MAX_PLAYERS];

PlayerData *getPlayerData(int p) {
	return &players[p];
}


// - Camera stuff -
static CameraData cameras[MAX_PLAYERS];

CameraData *getCameraData(int c) {
	return &cameras[c];
}


// - Corner management -
int addCorner(int p) {
	PlayerData *player = &players[p];
	
	if (player->index >= WALL_SIZE - 1) {
		printf("Reached maximum wall for Player%d\n", p);
		resetWorld();	/// TODO: Make update reset the world through return values. But first you have to get addCorner out of input
		return 0;
	}
	
	(player->index)++;
	player->trace[player->index] = player->position;
	return 1;
}


// - Reset -
void resetPlayer(int p) {
	memset(players[p].trace, 0, sizeof(players[p].trace));
	players[p].trace[0] = players[p].position;
	players[p].index = 0;

	cameras[p].rotation = (fVec3){0, 0, 0};
	cameras[p].targetRotation = (fVec3){
		(players[p].direction) / 4.0f * 360.0f,
		-30.f,
		0};
	cameras[p].position = (fVec3){-250, -300, 250};
	cameras[p].targetPosition = (fVec3){
		-players[p].position.x + sin(cameras[p].targetRotation.x / 180 * M_PI) * 25,
		-10,
		players[p].position.y - cos(cameras[p].targetRotation.x / 180 * M_PI) * 25};
	cameras[p].input = ' ';
}

void resetWorld(void) {
	players[0].position = (iVec2){ WORLD_SIZE / 4,		WORLD_SIZE / 2};
	players[1].position = (iVec2){ WORLD_SIZE / 4 * 3, 	WORLD_SIZE / 2};
	players[2].position = (iVec2){ WORLD_SIZE / 2, 		WORLD_SIZE / 4 * 3};
	players[3].position = (iVec2){ WORLD_SIZE / 2, 		WORLD_SIZE / 4};

	players[0].direction = RIGHT;
	players[1].direction = LEFT;
	players[2].direction = DOWN;
	players[3].direction = UP;

	players[0].color = (fVec3){0, 0, 1};
	players[1].color = (fVec3){1, 0, 0};
	players[2].color = (fVec3){0, 1, 0};
	players[3].color = (fVec3){1, 1, 1};
	
	for(int p=0; p<MAX_PLAYERS; p++) {
		resetPlayer(p);
	}
}
