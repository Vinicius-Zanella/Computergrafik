#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/world.h"

int spectator = 0;

// - Custom input handling -
static const Input inputOne = {'W', 'A', 'S', 'D', 'Q', 'E'};
static const Input inputTwo = {'I', 'J', 'K', 'L', 'U', 'O'};

Input getInput(int i) {
	return (i == 0) ? inputOne : inputTwo;
}


// - Player stuff -
static PlayerData  player_one = { 0 };
static PlayerData  player_two = { 0 };

PlayerData *getPlayerData(int p) {
	return (p == 0) ? &player_one : &player_two;
}


// - Camera stuff -
static CameraData camera_one = { 0 };
static CameraData camera_two = { 0 };

CameraData *getCameraData(int c) {
	return (c == 0) ? &camera_one : &camera_two;
}


// - Billboard stuff -
static BillboardData billboard_one = { 0 };
static BillboardData billboard_two = { 0 };

BillboardData *getBillboardData(int b) {
	return (b == 0) ? &billboard_one : &billboard_two;
}


// - Corner management -
void addCorner(int p) {
	PlayerData *player = (p == 0) ? &player_one : &player_two;
	if (player->index >= WALL_SIZE-1) {
		printf("Reached maximum wall for Player%d\n", p);
		resetWorld();	///TODO should anyone reset world or just update? consider returning an error so that update resets
		return;
	}
	
	(player->index)++;
	player->trace[player->index] = player->position;
}


// - Reset -
void resetWorld(void) {
	player_one.position = (iVec2){ WORLD_SIZE / 4, WORLD_SIZE / 2};
	memset(player_one.trace, 0, sizeof(player_one.trace));
	player_one.trace[0] = player_one.position;
	player_one.direction = RIGHT;
	player_one.index = 0;

	player_two.position = (iVec2){ WORLD_SIZE / 4 * 3, WORLD_SIZE / 2};
	memset(player_two.trace, 0, sizeof(player_two.trace));
	player_two.trace[0] = player_two.position;
	player_two.direction = LEFT;
	player_two.index = 0;

	camera_one.rotation = (fVec3){0, 0, 0};
	camera_one.targetRotation = (fVec3){(player_one.direction) / 4.0f * 360.0f, 30.f, 0};
	camera_one.position = (fVec3){-250, -300, 250};
	camera_one.targetPosition = (fVec3){-player_one.position.x + sin(camera_one.targetRotation.x / 180 * M_PI) * 25, -10, player_one.position.y - cos(camera_one.targetRotation.x / 180 * M_PI) * 25};
	//printf("Player position: (%d, %d), tarRotation: %.1f\n", player_one.position.x, player_one.position.y, camera_one.targetRotation.x);
	camera_one.input = ' ';
	camera_two.rotation = (fVec3){0, 0, 0};
	camera_two.targetRotation = (fVec3){player_two.direction / 4.0f * 360.0f, 30.0f, 0};
	camera_two.position = (fVec3){-250, -300, 250};
	camera_two.targetPosition = (fVec3){-player_two.position.x + sin(camera_two.targetRotation.x / 180 * M_PI) * 25, -10, player_two.position.y - cos(camera_two.targetRotation.x / 180 * M_PI) * 25};
}

/// TODO: Reset all players
