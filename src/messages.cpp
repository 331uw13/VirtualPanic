#include <stdio.h>
#include <stdarg.h>

#include "messages.hpp"

namespace vpanic {

	void message(MType t_type, const char* t_msg, ...) {
		printf(" \033[32m(VirtualPanic)\033[%im [%c] \033[90m->\033[0m ", 
				_msg_colors[(int)t_type], _type_chr[(int)t_type]);

		// i will make this better later...

		va_list ap;
		va_start(ap, t_msg);

		while(*t_msg) {	
			char ch = *t_msg++;
			if(ch == '%') {
				char next_ch = *t_msg++;
				switch(next_ch) {
					case 'i':
						printf("%i", va_arg(ap, int));
						break;
					
					case 'f':
						printf("%f", va_arg(ap, double));
						break;

					case 's':
						printf("%s", va_arg(ap, char*));
						break;

					default:
						if(next_ch != '0') {
							printf("\033[3%cm", next_ch);
						}
						else {
							printf("\033[0m");
						}
						break;
				}
			}
			else {
				printf("%c", ch);
			}
		}

		va_end(ap);
		printf("\033[0m\n");
	}
}


