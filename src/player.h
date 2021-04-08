#ifndef VIRTUAL_PANIC__PLAYER_H
#define VIRTUAL_PANIC__PLAYER_H

#include "types.h"
#include "camera.h"


typedef struct {
	Vector3 position;
	VCamera camera;
	float height;
	float gravity;
	float speed;
} VPlayer;


//void VSetMaxPlayers(uint16 n); // not needed yet.
VPlayer* VCreateNewPlayer();
void VDestroyPlayer();
VPlayer* VGetPlayer();


#endif
