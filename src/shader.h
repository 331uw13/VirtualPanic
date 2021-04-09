#ifndef VIRTUAL_PANIC__SHADER_H
#define VIRTUAL_PANIC__SHADER_H

#include "types.h"
#include "matrix.h"


typedef uint16 VShader;

VShader VCreateShader(const char* src);
VShader VGetFirstCreatedShader();
void VDestroyShader(VShader* id);
void VShaderAddUniformBlockBinding(VShader shader, const char* block_name, uint32 binding_point);

void VShaderSetMatrix(VShader shader, uint32 uniform_index, VMatrix* m);
void VShaderSetColor(VShader shader, uint32 uniform_index, VColor* c);
void VShaderSetVector3(VShader shader, uint32 uniform_index, Vector3* v);
void VShaderSetFloat(VShader shader, uint32 uniform_index, float f);
void VShaderSetInt(VShader shader, uint32 uniform_index, int i);


#endif
