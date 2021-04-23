#include <stddef.h>
#include "libs/gl3w.h"

#include "texture.h"
#include "internal/core.h"


VTexture VLoadTexture(const char* filename) {
	
	VTexture tx;
	tx.type = GL_TEXTURE_2D;
	glGenTextures(1, &tx.id);
	glBindTexture(tx.type, tx.id);

	glTexParameteri(tx.type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tx.type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(tx.type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(tx.type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 5);
	
	VCoreGenerateImage(filename, tx.type);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tx;
}


void VDestroyTexture(VTexture* texture) {
	if(texture == NULL) { return; }
	glDeleteTextures(1, &texture->id);
	texture->id = 0;
	texture->type = 0;
}


