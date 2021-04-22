#ifndef VIRTUAL_PANIC_INTERNAL__CORE_H
#define VIRTUAL_PANIC_INTERNAL__CORE_H

#include "../types.h"

// These functions should not be used outside library.


#define VCORE_COMPILE_INTERNAL_SHADER 0
#define VCORE_COMPILE_USER_SHADER 1
#define VCORE_MODEL_MATRIX_INDEX 0

uint32 VCoreCompileShaderModule(const char* src, uint32 type, uint8 flag);
uint32 VCoreLinkModules(uint32* modules, uint32 size);
uint32 VCoreLinkToInternalVertexModule(uint32 module_id);

void VCoreCompileDefaultVertexModule();
uint32 VCoreGetVertexModule();

void VCoreGenerateImage(const char* filename, int type);


#endif
