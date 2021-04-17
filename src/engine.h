#ifndef VIRTUAL_PANIC__ENGINE_H
#define VIRTUAL_PANIC__ENGINE_H

#include "status.h"
#include "player.h"
#include "uniform_buffer.h"


// NOTES:
// - use some sort of quadtree for collision detection


void VEngineInit(const char* title);
void VEngineStart();
void VEngineFree();
void VEngineShutdown();
int VGetEngineStatus();
void VEngineSetVSyncEnabled(uint8 b);
void VEngineSetCameraEnabled(uint8 b);
void VEngineSetf(uint32 flag, float value);
float VEngineGetf(uint32 flag);

void VEngineSetupCallback(void(*user_callback)());
void VEngineUpdateCallback(void(*user_callback)(double));

uint8 VKeyDown(uint32 k);
uint8 VKeyUp(uint32 k); // DELETE THIS


VMatrix VEngineGetProjection();
VMatrix VEngineGetView();


#endif
