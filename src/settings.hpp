#pragma once


namespace vpanic {
	
	enum {
	
		MSG_INFO = 1 << 0,
		MSG_OK = 1 << 1,
		MSG_WARNING = 1 << 2,
		MSG_ERROR = 1 << 3,
		MSG_DEBUG = 1 << 4,
		MSG_NO_STDOUT = 1 << 5,
	
		ENGINE_OK = 1 << 6,
		ENGINE_INIT_OK = 1 << 7,
		ENGINE_QUIT = 1 << 8,
		ENGINE_KEEP_LOOP = 1 << 9,
		ENGINE_LOCK_MOUSE = 1 << 10,
		FULLSCREEN = 1 << 11,
		VSYNC = 1 << 12,
		CONSOLE_GET_FOCUS = 1 << 13,

	};

	enum {
		
		MSG_INFO_COLOR = 0x2d98da,
		MSG_OK_COLOR = 0x20bf6b,
		MSG_WARNING_COLOR = 0xfa8231,
		MSG_ERROR_COLOR = 0xeb3b5a,
		MSG_DEBUG_COLOR = 0xa55eea,
		MSG_UNKNOWN_COLOR = 0x888888,

		ENGINE_UBO_BINDING = 83,
		CLOCKWISE = 1,
		COUNTER_CLOCKWISE = 2,
		DOUBLE_SIDE = 3,
		INCLUDE_SUB_DIRECTORIES = 4,



	};

}

