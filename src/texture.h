#ifndef VIRTUAL_PANIC__TEXTURE_H
#define VIRTUAL_PANIC__TEXTURE_H

#include "types.h"

typedef uint32 VTexture;

VTexture VLoadTexture(const char* filename);
void VDestroyTexture(VTexture* texture);


#endif
