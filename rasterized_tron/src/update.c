#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/vector.h"
#include "../include/update.h"

// --- Constants ---
#define CAMERA_FACTOR 7.5f
#define CAMERA_APPROXIMATION 0.1f

#define SPECTATOR_SPEED 50.f
#define SPECTATOR_TURN 42.f

#define PLAYER_SPEED 1;

// --- Global ---
static int playerCount = 0;
static int freeze = 0;
static int freezeTime = 5;
static float timer = 0;

// -- Function declaration --
static void playerInput(int p);
static int isOutOfBounds(iVec2 pos);
static int collidedWithWall(iVec2 pos);
static void gameOver(float dt);



// --- Entry Point ---
void initGame(int count) {
	playerCount = count;
	resetWorld();
}

void game_update(float dt) {
	if (freeze == 1) {
		gameOver(dt);
		return;
	}
	
	for (int p=0; p<playerCount; p++) {
		playerInput(p);

		// -- Camera movement --
		CameraData *camera = getCameraData(p);
		float delta = camera->targetPosition.x - camera->position.x;
		if (-CAMERA_APPROXIMATION < delta && delta < CAMERA_APPROXIMATION) delta = 0;
		else camera->position.x += delta / CAMERA_FACTOR;
			
		delta = camera->targetPosition.y - camera->position.y;
		if (-CAMERA_APPROXIMATION < delta && delta < CAMERA_APPROXIMATION) camera->position.y = camera->targetPosition.y;
		else camera->position.y += delta / CAMERA_FACTOR;

		delta = camera->targetPosition.z - camera->position.z;
		if (-CAMERA_APPROXIMATION < delta && delta < CAMERA_APPROXIMATION) camera->position.z = camera->targetPosition.z;
		else camera->position.z += delta / CAMERA_FACTOR;

		delta = camera->targetRotation.x - camera->rotation.x;
		if (-CAMERA_APPROXIMATION < delta && delta < CAMERA_APPROXIMATION) camera->rotation.x = camera->targetRotation.x;
		else camera->rotation.x += delta / CAMERA_FACTOR;
			
		// -- Physics --
		PlayerData *player = getPlayerData(p);
		if (collidedWithWall(player->position)) {
			player->status = DEAD;
			gameOver(dt);
		}
	
		if (isOutOfBounds(player->position)) {
			player->status = DEAD;
			gameOver(dt);
		}
	}
}



// -- Functions --
static void playerInput(int p) {
	PlayerData *player = getPlayerData(p);
	CameraData *camera = getCameraData(p);
	switch (player->direction) {
		case UP:
			player->position.y += PLAYER_SPEED;
			camera->targetPosition.z += PLAYER_SPEED;
			break;
		case LEFT:
			player->position.x -= PLAYER_SPEED;
			camera->targetPosition.x += PLAYER_SPEED;
			break;
		case DOWN:
			player->position.y -= PLAYER_SPEED;
			camera->targetPosition.z -= PLAYER_SPEED;
			break;
		case RIGHT:
			player->position.x += PLAYER_SPEED;
			camera->targetPosition.x -= PLAYER_SPEED;
			break;
	}
}

static int isOutOfBounds(iVec2 pos) {
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

static int collidedWithWall(iVec2 pos) {
	for (int p=0; p<playerCount; p++) {
		PlayerData *player = getPlayerData(p);
		iVec2 *trace = player->trace;

		for (int c=0; c<player->index; c++) {
			iVec2 a = trace[c];
			iVec2 b = trace[c+1];
			
			// - Horizontal -
			if (a.y == pos.y && b.y == pos.y) {
				if (fmin(a.x, b.x) < pos.x && pos.x < fmax(a.x, b.x))
					return 1;
			}
			// - Vertical -
			if (a.x == pos.x && pos.x == b.x) {
				if (fmin(a.y, b.y) < pos.y && pos.y < fmax(a.y, b.y))
					return 1;
			}
		}

		// Active segment
		if (trace[player->index].y == player->position.y && pos.y == player->position.y) {
			if (fmin(trace[player->index].x, player->position.x) < pos.x && pos.x < fmax(trace[player->index].x, player->position.x))
				return 1;
		}
		if (trace[player->index].x == player->position.x && pos.x == player->position.x) {
			if (fmin(trace[player->index].y, player->position.y) < pos.y && pos.y < fmax(trace[player->index].y, player->position.y))
				return 1;
		}
	}
	return 0;
}

static void gameOver(float dt) {
	freeze = 1;
	timer += dt;

	if (timer >= freezeTime) {
		freeze = 0;
		timer = 0;
		resetWorld();
	}
}
