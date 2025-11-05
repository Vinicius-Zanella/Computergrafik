#pragma once

#include <GLFW/glfw3.h>
#include "vector.h"

void initRender(void);

void resize(GLFWwindow *window, int width, int height);

void render(void);
