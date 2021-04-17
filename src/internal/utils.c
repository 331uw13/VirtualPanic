#include <stdlib.h>
#include <errno.h>

#include "../messages.h"
#include "utils.h"


uint8 VCoreUAllocBack(void*** ptr, uint32 size, void** data, uint32 data_size) {
	uint8 res = 1;

	void** tmpptr = realloc(*ptr, size + data_size);
	if(tmpptr != NULL) {
		*ptr = tmpptr;
		if(*data == NULL) {
			*data = malloc(data_size);
			if(*data != NULL) {
				goto finish;
			}
		}
		else {
			VMessage(VMSG_WARNING, "Detected memory leak!");
		}
	}

	VMessage(VMSG_ERROR, "Failed to allocate memory!");
	if(errno == ENOMEM) {
		VMessage(VMSG_ERROR, "Out of memory!");
	}

finish:
	return res;
}

