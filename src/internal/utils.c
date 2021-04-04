#include "utils.h"

void VSetSFlag(int* status, int flag, uint8 on_off) {
	on_off ? (*status |= flag) : (*status &= ~flag);
}



