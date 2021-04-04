#ifndef VIRTUAL_PANIC__ENGINE_H
#define VIRTUAL_PANIC__ENGINE_H

#include "status.h"
#include "shape.h"
#include "player.h"

// NOTES:
// - try to do matrices in shader?
// - use some sort of quadtree for collision detection


void VEngineInit(const char* title);
void VEngineStart();
void VEngineFree();
void VEngineShutdown();
int VGetEngineStatus();
void VEngineSetCameraEnabled(uint8 b);
void VEngineSetTimeScale(uint32 time_scale);

void VEngineSetupCallback(void(*user_callback)());
void VEngineUpdateCallback(void(*user_callback)(double));

VPlayer* VGetPlayer();
uint8 VKeyDown(char c);

uint32 VLoadShader(const char* src);
void VUnloadShader(uint32* id);




#endif
