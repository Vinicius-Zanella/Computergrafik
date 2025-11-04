#include <math.h>
#include "../include/world.h"
#include "../include/vector.h"
#include "../include/update.h"

// -- Function declaration --
int isOutOfBounds(iVec2 pos);
int collidedWithWall(iVec2 pos);

// --- Entry Point ---
void update(void) {
	///TODO loop both
	PlayerData *player = getPlayerData(1);
	switch (player->input) {	///TODO define speed instead of magic number 1
		case 'W': player->position.y += 1; break;
		case 'A': player->position.x -= 1; break;
		case 'S': player->position.y -= 1; break;
		case 'D': player->position.x += 1; break;
	}
	if (collidedWithWall(player->position)) resetWorld();
	if (isOutOfBounds(player->position)) resetWorld();
	
	player = getPlayerData(2);
	switch (player->input) {	///TODO define speed instead of magic number 1
		case 'I': player->position.y += 1; break;
		case 'J': player->position.x -= 1; break;
		case 'K': player->position.y -= 1; break;
		case 'L': player->position.x += 1; break;
	}	
	if (collidedWithWall(player->position)) resetWorld();
	if (isOutOfBounds(player->position)) resetWorld();

	// - Bounds -
	
	// - Collision -
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
