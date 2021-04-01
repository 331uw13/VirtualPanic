#include <stdio.h>
#include <stdarg.h>

#include "messages.hpp"
#include "console.hpp"

//#define NAME_TO_STR(x) #x

namespace vpanic {

	void message(MType t_type, const char* t_msg, ...) {
		
		char buf[600];

		va_list ap;
		va_start(ap, t_msg);
	
		vsnprintf(buf, sizeof(buf), t_msg, ap);
		buf[sizeof(buf)-1] = '\0';
		
		printf(" (VirtualPanic) -> [%s]: %s\n", _msg_types[static_cast<int>(t_type)], buf);
		//Console::instance().print(_msg_colors[(int)t_type], "[%s]: %s\n", _msg_types[static_cast<int>(t_type)], buf);
		
		va_end(ap);
	}
}


