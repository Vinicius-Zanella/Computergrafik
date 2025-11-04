#include <stdio.h>
#include <string.h>
#include "../include/world.h"

static PlayerData  player_one = { 0 };
static PlayerData  player_two = { 0 };

///TODO camera

// Billboard

PlayerData *getPlayerData(int p) {
	return (p == 1) ? &player_one : &player_two;
}

///TODO update player

void addCorner(int p) {
	PlayerData *player = (p == 1) ? &player_one : &player_two;
	if (player->index >= WALL_SIZE-1) {
		printf("Reached maximum wall for Player%d\n", p);
		resetWorld();	///TODO should anyone reset world or just update? consider returning an error so that update resets
		return;
	}
	
	(player->index)++;
	player->trace[player->index] = player->position;
}

void resetWorld(void) {
	player_one.position = (iVec2){ WORLD_SIZE / 4, WORLD_SIZE / 2};
	memset(player_one.trace, 0, sizeof(player_one.trace));
	player_one.trace[0] = player_one.position;
	player_one.input = 'D';
	player_one.index = 0;

	player_two.position = (iVec2){ WORLD_SIZE / 4 * 3, WORLD_SIZE / 2};
	memset(player_two.trace, 0, sizeof(player_two.trace));
	player_two.trace[0] = player_two.position;
	player_two.input = 'J';
	player_two.index = 0;
}
