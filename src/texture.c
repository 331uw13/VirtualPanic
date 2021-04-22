#include <stddef.h>
#include "libs/gl3w.h"

#include "texture.h"
#include "internal/core.h"


VTexture VLoadTexture(const char* filename) {
	
	VTexture tx = 0;
	glGenTextures(1, &tx);
	glBindTexture(GL_TEXTURE_2D, tx);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 5);
	
	VCoreGenerateImage(filename, GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tx;
}


void VDestroyTexture(VTexture* texture) {
	if(texture == NULL) { return; }
	glDeleteTextures(1, texture);
	*texture = 0;
}


