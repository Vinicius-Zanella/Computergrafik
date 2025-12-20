#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/vector.h"
#include "../include/update.h"

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
	} else {
		for (int p=0; p<playerCount; p++) {
			
			playerInput(p);

			// -- Camera movement --
			CameraData *camera = getCameraData(p);
			float delta = camera->targetPosition.x - camera->position.x;
			camera->position.x += delta / 7.5f;
			delta = camera->targetPosition.y - camera->position.y;
			camera->position.y += delta / 7.5f;
			delta = camera->targetPosition.z - camera->position.z;
			camera->position.z += delta / 7.5f;

			delta = camera->targetRotation.x - camera->rotation.x;
			camera->rotation.x += delta / 7.5f;
			
			
			// -- Physics --
			PlayerData *player = getPlayerData(p);
			if (collidedWithWall(player->position)) resetWorld();
	
			if (isOutOfBounds(player->position)) resetWorld();
		}
	}
}

void spectatorInput(float dt) {
	float speed = 50.0f;
	float turn = 36.0f;
	// Camera
	CameraData *camera = getCameraData(1);
	Input i = getInput(1);
	if (i.Upwa == camera->input) {
		camera->position.z += speed * cos(camera->rotation.x / 180.f * M_PI) * dt;
		camera->position.x -= speed * sin(camera->rotation.x / 180.f * M_PI) * dt;
	} else if (i.Left == camera->input) {
		camera->position.x += speed * cos(camera->rotation.x / 180.f * M_PI) * dt;
		camera->position.z += speed * sin(camera->rotation.x / 180.f * M_PI) * dt;
	} else if (i.Down == camera->input) {
		camera->position.z -= speed * cos(camera->rotation.x / 180.f * M_PI) * dt;
		camera->position.x += speed * sin(camera->rotation.x / 180.f * M_PI) * dt;
	} else if (i.Rigt == camera->input) {
		camera->position.x -= speed * cos(camera->rotation.x / 180.f * M_PI) * dt;
		camera->position.z -= speed * sin(camera->rotation.x / 180.f * M_PI) * dt;} else
	if (i.Qsud == camera->input) camera->position.y -= speed * dt; else
	if (i.Esud == camera->input) camera->position.y += speed * dt;
	i = getInput(2);
	if (i.Upwa == camera->input) camera->rotation.y -= turn * dt; else
	if (i.Left == camera->input) camera->rotation.x -= turn * dt; else
	if (i.Down == camera->input) camera->rotation.y += turn * dt; else
	if (i.Rigt == camera->input) camera->rotation.x += turn * dt; else
	if (i.Qsud == camera->input) camera->rotation.z -= turn * dt; else
	if (i.Esud == camera->input) camera->rotation.z += turn * dt;
}

void playerInput(int p) {				
	PlayerData *player = getPlayerData(p);
	CameraData *camera = getCameraData(p);
	///TODO define speed instead of magic number 1
	if (player->direction == UP) {
		player->position.y += 1;
		camera->targetPosition.z += 1;
	} else
	if (player->direction == LEFT) {
		player->position.x -= 1;
		camera->targetPosition.x += 1;
	} else
	if (player->direction == DOWN) {
		player->position.y -= 1;
		camera->targetPosition.z -= 1;
	} else
	if (player->direction == RIGHT) {
		player->position.x += 1;
		camera->targetPosition.x -= 1;
	}
}

float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

int isOutOfBounds(iVec2 pos) {
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

int collidedWithWall(iVec2 pos) {
	for (int p=1; p<playerCount; p++) {
		PlayerData *player = (p == 1) ? getPlayerData(1) : getPlayerData(2);
		iVec2 *trace = (p == 1) ? getPlayerData(1)->trace : getPlayerData(2)->trace;

		for (int c=0; c<player->index; c++) {
			// - Horizontal -
			if (trace[c].y == pos.y && trace[c+1].y == pos.y) {
				if (fmin(trace[c].x, trace[c+1].x) < pos.x && pos.x < fmax(trace[c].x, trace[c+1].x))
					return 1;
			}
			// - Vertical -
			if (trace[c].x == pos.x && pos.x == trace[c+1].x) {
				if (fmin(trace[c].y, trace[c+1].y) < pos.y && pos.y < fmax(trace[c].y, trace[c+1].y))
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
