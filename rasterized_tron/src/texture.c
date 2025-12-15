#include <stdio.h>
#include "../include/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

void initTexture(TexStruct *texture) {
	unsigned char *data = stbi_load(texture->filename, &texture->texWidth, &texture->texHeight, &texture->texChannels, STBI_rgb_alpha);
	if (!data) {
		fprintf(stderr, "Failed to load texture: %s\n", texture->filename);
		return;
	}	
	
	glGenTextures(1, &texture->texture);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->texWidth, texture->texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
	stbi_image_free(data);
}
