#ifndef VIRTUAL_PANIC_INTERNAL__CORE_H
#define VIRTUAL_PANIC_INTERNAL__CORE_H

#include "../types.h"

// These functions should not be used outside library.


#define VCORE_COMPILE_INTERNAL_SHADER 0
#define VCORE_COMPILE_USER_SHADER 1

uint32 VCoreCompileShaderModule(const char* src, uint32 type, uint8 flag);
uint32 VCoreLinkShaderModule(uint32 module_id);
void VCoreCompileDefaultVertexModule();


#endif
