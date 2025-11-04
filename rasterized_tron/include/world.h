#pragma once

#include "../include/vector.h"

#define WORLD_SIZE 500
#define WALL_SIZE 100

typedef struct {
	iVec2 position;
	char input;
	int index;
	iVec2 trace[WALL_SIZE];
} PlayerData;

PlayerData *getPlayerData(int p);
//void updatePlayerData(void);

///TODO Camera data

void addCorner(int p);

void resetWorld(void);
