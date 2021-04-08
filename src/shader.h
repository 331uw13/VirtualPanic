#ifndef VIRTUAL_PANIC__SHADER_H
#define VIRTUAL_PANIC__SHADER_H

#include "types.h"


typedef uint32 VShader;


uint32 VCreateShader(const char* src);
VShader VGetFirstCreatedShader();
void VDestroyShader(uint32* id);
void VShaderAddUniformBlockBinding(uint32 shader_id, const char* block_name, uint32 binding_point);





#endif
