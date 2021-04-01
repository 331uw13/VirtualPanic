#include <stdio.h>
#include <stdarg.h>

#include "messages.hpp"
#include "console.hpp"
#include "settings.hpp"

namespace vpanic {

	void message(const int t_type, const char* t_msg, ...) {
		// check if user blocked this type of message
		const State& console_state = console::copy_state();
		if(console_state[t_type]) { return; }


		char buf[1024];
		va_list ap;
		va_start(ap, t_msg);
		vsnprintf(buf, sizeof(buf), t_msg, ap);
		buf[sizeof(buf)-1] = '\0';
		

		const char* type_str = "UNKNOWN";
		uint32_t type_color = MSG_UNKNOWN_COLOR;

		switch(t_type) {

			case MSG_INFO:
				type_str = "INFO";
				type_color = MSG_INFO_COLOR;
				break;
			
			case MSG_OK:
				type_str = "OK";
				type_color = MSG_OK_COLOR;
				break;

			case MSG_WARNING:
				type_str = "WARNING";
				type_color = MSG_WARNING_COLOR;
				break;

			case MSG_ERROR:
				type_str = "ERROR";
				type_color = MSG_ERROR_COLOR;
				break;

			case MSG_DEBUG:
				type_str = "DEBUG";
				type_color = MSG_DEBUG_COLOR;
				break;

			default: break;
		}


		console::print("<%x>[%s] --> %s", type_color, type_str, buf);
		if(!console_state[MSG_NO_STDOUT]) {
			printf(" (VirtualPanic) -> [%s]: %s\n", type_str, buf);
		}
		


		//Console::instance().print(_msg_colors[(int)t_type], "[%s]: %s\n", _msg_types[static_cast<int>(t_type)], buf);
		
		va_end(ap);
	}
}


