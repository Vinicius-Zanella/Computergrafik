#pragma once

#include <GLFW/glfw3.h>
#include "vector.h"

int getStatus();

// void initRender(void);

void initGui();

void gui_resize(GLFWwindow *window, int width, int height);

void gui_render(void);

void gui_update(void);

void gui_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
