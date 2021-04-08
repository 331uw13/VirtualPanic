#include <stdlib.h>

#include "player.h"
#include "messages.h"
#include "status.h"


static VPlayer* player;



VPlayer* VCreateNewPlayer() {
	if(player == NULL) {
		if((player = (VPlayer*)malloc(sizeof(VPlayer))) != NULL) {
			player->position.x = 0.0f;
			player->position.y = 0.0f;
			player->position.z = 0.0f;
			player->height = VPLAYER_DEFAULT_HEIGHT;
			player->speed = VPLAYER_DEFAULT_SPEED;
			player->gravity = 0.0f; // todo
			player->camera.position.x = 0.0f;
			player->camera.position.y = VPLAYER_DEFAULT_HEIGHT;
			player->camera.position.z = 0.0f;
			player->camera.sensetivity = 5.0f;
			player->camera.yaw = -90.f;
			player->camera.pitch = 0.0f;
			player->camera.noclip = FALSE; // todo
		}
		else {
			VMessage(VMSG_ERROR, "Failed to allocate memory for new player!");
		}
	}
	return player;
}


void VDestroyPlayer() {
	free(player);
	player = NULL;
}


VPlayer* VGetPlayer() {
	return player;
}


