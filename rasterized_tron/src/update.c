#include <stdio.h>
#include <math.h>
#include "../include/world.h"
#include "../include/vector.h"
#include "../include/update.h"

// -- Function declaration --
int isOutOfBounds(iVec2 pos);
int collidedWithWall(iVec2 pos);

// --- Entry Point ---
void update(float dt) {
	if (spectator == 1) {
		// Camera
		CameraData *camera = getCameraData(1);
		Input i = getInput(1);
		if (i.Upwa == camera->input) camera->position.z += 2.5f * dt; else
		if (i.Left == camera->input) camera->position.x += 2.5f * dt; else
		if (i.Down == camera->input) camera->position.z -= 2.5f * dt; else
		if (i.Rigt == camera->input) camera->position.x -= 2.5f * dt; else
		if (i.Qsud == camera->input) camera->position.y -= 2.5f * dt; else
		if (i.Esud == camera->input) camera->position.y += 2.5f * dt;
		i = getInput(2);
		if (i.Upwa == camera->input) camera->rotation.y -= 42 * dt; else
		if (i.Left == camera->input) camera->rotation.x -= 42 * dt; else
		if (i.Down == camera->input) camera->rotation.y += 42 * dt; else
		if (i.Rigt == camera->input) camera->rotation.x += 42 * dt; else
		if (i.Qsud == camera->input) camera->rotation.z -= 1 * dt; else
		if (i.Esud == camera->input) camera->rotation.z += 1 * dt;
		
	printf("Camera: (%.1f, %.1f, %.1f)(%.1f, %.1f, %.1f)\n", camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y, camera->rotation.z);
	} else {
		for (int c=1; c<=2; c++) {
			///TODO outsource input handling to input file
			PlayerData *player = getPlayerData(c);
			const Input i = getInput(c);
			///TODO define speed instead of magic number 1
			if (i.Upwa == player->input) player->position.y += 1; else
			if (i.Left == player->input) player->position.x -= 1; else
			if (i.Down == player->input) player->position.y -= 1; else
			if (i.Rigt == player->input) player->position.x += 1;
	
			if (collidedWithWall(player->position)) resetWorld();
	
			if (isOutOfBounds(player->position)) resetWorld();
		}
	}
}

void initGame(void) {
	resetWorld();
}

int isOutOfBounds(iVec2 pos) {
	return (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE);
}

int collidedWithWall(iVec2 pos) {
	for (int p=1; p<=2; p++) {
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
