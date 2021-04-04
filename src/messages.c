#include <stdarg.h>
#include <stdio.h>
#include "messages.h"

static int pline_length = 0;

void VMessage(int type, const char* msg, ...) {
	va_list a;
	va_start(a, msg);
	char buf[VMESSAGES_BUFFER_SIZE];
	vsnprintf(buf, VMESSAGES_BUFFER_SIZE, msg, a);

	const char* type_string = "Unknown";
	switch(type) {
		case VMSG_OK:
			type_string = "Ok";
			break;
		case VMSG_INFO:
			type_string = "Info";
			break;
		case VMSG_WARNING:
			type_string = "Warning";
			break;
		case VMSG_ERROR:
			type_string = "Error";
			break;
		case VMSG_DEBUG:
			type_string = "Debug";
			break;
		default: break;
	}

	// As you can see there is a problem with this because printf counts the escape sequences too. :/
	pline_length = printf(" \033[%im(VirtualPanic)(%s)\033[0m --> %s\n", type, type_string, buf);
	pline_length -= 10;
	va_end(a);
}

void VMessageSameLine(const char* msg) {
	// Go to previous line and end of the now current line.
	printf("\033[1F\033[%iC%s\n", pline_length, msg);
}





