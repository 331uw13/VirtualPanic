#ifndef VIRTUAL_PANIC__SHADER_H
#define VIRTUAL_PANIC__SHADER_H

#include "types.h"
#include "matrix.h"


typedef uint16 VShader;

VShader VCreateShader(const char* src);
VShader VGetFirstCreatedShader();
void VDestroyShader(VShader* id);
void VShaderAddUniformBlockBinding(VShader shader, const char* block_name, uint32 binding_point);

void VShaderSetMatrix(VShader shader, const char* name, VMatrix* m);
void VShaderSetVector3(VShader shader, const char* name, Vector3* v);
void VShaderSetVector4(VShader shader, const char* name, Vector4* v);
void VShaderSetFloat(VShader shader, const char* name, float f);
void VShaderSetInt(VShader shader, const char* name, int i);


#endif
