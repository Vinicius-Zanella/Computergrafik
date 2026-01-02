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
int playerCount = 0;

// -- Function declaration --
void spectatorInput(float dt);
void playerInput(int p);
float lerp(float a, float b, float t);
int isOutOfBounds(iVec2 pos);
int collidedWithWall(iVec2 pos);

// --- Entry Point ---
void initGame(int count) {
	playerCount = count;
	resetWorld();
}

void game_update(float dt) {
	///TODO remove spectator in the finished game
	if (spectator == 1) {
		spectatorInput(dt);
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
		if (collidedWithWall(player->position)) resetWorld();
	
		if (isOutOfBounds(player->position)) resetWorld();
	}
}


void spectatorInput(float dt) {
	// Camera
	CameraData *camera = getCameraData(0);
	const float angleYaw = camera->rotation.x / 180.f * M_PI;
	const float cosYaw = cosf(angleYaw);
	const float sinYaw = sinf(angleYaw);
	
	Input i = getInput(0);
	if (i.Upwa == camera->input) {
		camera->position.z += SPECTATOR_SPEED * cosYaw * dt;
		camera->position.x -= SPECTATOR_SPEED * sinYaw * dt;
	} else if (i.Left == camera->input) {
		camera->position.x += SPECTATOR_SPEED * cosYaw * dt;
		camera->position.z += SPECTATOR_SPEED * sinYaw * dt;
	} else if (i.Down == camera->input) {
		camera->position.z -= SPECTATOR_SPEED * cosYaw * dt;
		camera->position.x += SPECTATOR_SPEED * sinYaw * dt;
	} else if (i.Rigt == camera->input) {
		camera->position.x -= SPECTATOR_SPEED * cosYaw * dt;
		camera->position.z -= SPECTATOR_SPEED * sinYaw * dt;} else
	if (i.Qsud == camera->input) camera->position.y -= SPECTATOR_SPEED * dt; else
	if (i.Esud == camera->input) camera->position.y += SPECTATOR_SPEED * dt;
	i = getInput(1);
	if (i.Upwa == camera->input) camera->rotation.y -= SPECTATOR_TURN * dt; else
	if (i.Left == camera->input) camera->rotation.x -= SPECTATOR_TURN * dt; else
	if (i.Down == camera->input) camera->rotation.y += SPECTATOR_TURN * dt; else
	if (i.Rigt == camera->input) camera->rotation.x += SPECTATOR_TURN * dt; else
	if (i.Qsud == camera->input) camera->rotation.z -= SPECTATOR_TURN * dt; else
	if (i.Esud == camera->input) camera->rotation.z += SPECTATOR_TURN * dt;
}

///TODO make delta time based?
void playerInput(int p) {				
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

int isOutOfBounds(iVec2 pos) {
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

int collidedWithWall(iVec2 pos) {
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
