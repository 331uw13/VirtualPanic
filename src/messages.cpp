#include <cstdio>
#include <cstring>

#include "messages.hpp"

namespace VPanic {
	
	void message(const char* t_text, MsgType t_type) {
		printf("\x1b[32m%s\x1b[90m -> \x1b[%im[%c]: %s\x1b[0m\n", 
				m_prefix, m_colors[(int)t_type], m_visual[(int)t_type], t_text);
	}

	void message(const char* t_text, const char* t_text2, MsgType t_type) {
		printf("\x1b[32m%s\x1b[90m -> \x1b[%im[%c]: %s \"%s\"\x1b[0m\n", 
				m_prefix, m_colors[(int)t_type], m_visual[(int)t_type], t_text, t_text2);
		
	}
}
