#pragma once

#include "../include/vector.h"

#define WORLD_SIZE 500
#define WALL_SIZE 100

extern int spectator;


typedef struct {
	char Upwa;
	char Left;
	char Down;
	char Rigt;
	char Esud;
	char Qsud;
} Input;

Input getInput(int i);


typedef struct {
	iVec2 position;
	char input;
	int index;
	iVec2 trace[WALL_SIZE];
} PlayerData;

PlayerData *getPlayerData(int p);
//void updatePlayerData(void);


typedef struct {
	fVec3 position;
	fVec3 rotation;
	int input;
} CameraData;

CameraData *getCameraData(int c);

typedef struct {
	fVec3 position;
	fVec3 size;
	float angle;
}BillboardData;

BillboardData *getBillboardData(int b);


void addCorner(int p);

void resetWorld(void);
