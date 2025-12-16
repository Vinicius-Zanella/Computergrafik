#pragma once

#include <GLFW/glfw3.h>
#include "vector.h"

struct displayArea {
	fVec2 start;
	fVec2 end;
};

enum displayOrder {
	FULL,
	NORTH,
	SOUTH,
	EAST,
	WEST,
	NORTHEAST,
	SOUTHEAST,
	SOUTHWEST,
	NORTHWEST
};

extern struct displayArea displayPositions[];


void initRender(int _playerCount, int width, int height, struct displayArea *viewports);

void resize(GLFWwindow *window, int width, int height);

void game_render(void);
