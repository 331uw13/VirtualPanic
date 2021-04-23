#ifndef VIRTUAL_PANIC__TEXTURE_H
#define VIRTUAL_PANIC__TEXTURE_H

#include "types.h"

typedef struct {
	uint32 id;
	uint32 type;
} VTexture;


VTexture VLoadTexture(const char* filename);
void VDestroyTexture(VTexture* texture);


#endif
