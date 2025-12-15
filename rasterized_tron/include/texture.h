#pragma once

#include <GL/gl.h>

typedef struct {
	GLuint texture;
	int texWidth, texHeight, texChannels;
	const char *filename;
} TexStruct;

void initTexture(TexStruct *texture);
