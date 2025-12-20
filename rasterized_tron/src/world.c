#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/world.h"

int spectator = 0;

// - Custom input handling -
static const Input input[] = {
	{'W', 'A', 'S', 'D', 'Q', 'E'},
	{'I', 'J', 'K', 'L', 'U', 'O'},
};

Input getInput(int i) {
	return input[i];
}


// - Player stuff -
static PlayerData players[2];

PlayerData *getPlayerData(int p) {
	return &players[p];
}


// - Camera stuff -
static CameraData cameras[2];

CameraData *getCameraData(int c) {
	return &cameras[c];
}


// - Billboard stuff -
static BillboardData billboards[2];

BillboardData *getBillboardData(int b) {
	return &billboards[b];
}


// - Corner management -
void addCorner(int p) {
	PlayerData *player = &players[p];
	if (player->index >= WALL_SIZE-1) {
		printf("Reached maximum wall for Player%d\n", p);
		resetWorld();	///TODO should anyone reset world or just update? consider returning an error so that update resets
		return;
	}
	
	(player->index)++;
	player->trace[player->index] = player->position;
}


// - Reset -
void resetPlayer(int p) {
	memset(players[p].trace, 0, sizeof(players[p].trace));
	players[p].trace[0] = players[p].position;
	players[p].index = 0;

	cameras[p].rotation = (fVec3){0, 0, 0};
	cameras[p].targetRotation = (fVec3){(players[p].direction) / 4.0f * 360.0f, 30.f, 0};
	cameras[p].position = (fVec3){-250, -300, 250};
	cameras[p].targetPosition = (fVec3){-players[p].position.x + sin(cameras[p].targetRotation.x / 180 * M_PI) * 25, -10, players[p].position.y - cos(cameras[p].targetRotation.x / 180 * M_PI) * 25};
	cameras[p].input = ' ';
}

void resetWorld(void) {
	///TODO 2 is max player cap. I didn't want to make a world init
	players[0].position = (iVec2){ WORLD_SIZE / 4, WORLD_SIZE / 2};
	players[1].position = (iVec2){ WORLD_SIZE / 4 * 3, WORLD_SIZE / 2};

	players[0].direction = RIGHT;
	players[1].direction = LEFT;

	players[0].color = (fVec3){0, 0, 1};
	players[1].color = (fVec3){1, 0, 0};
	
	for(int p=0; p<2; p++) {
		resetPlayer(p);
	}
}
