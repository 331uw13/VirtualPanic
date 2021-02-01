#pragma once

namespace VPanic {

	enum class MType { // "message type"
		INFO,
		OK,
		BAD,
		WARNING,
		DEBUG
	};

	static const unsigned char _msg_colors[6] {
		34, // INFO
		32, // OK
		31, // BAD
		33, // WARNING
		35  // DEBUG
	};

	static const char _type_chr[6] {
		'i', // INFO
		'+', // OK
		'x', // BAD
		'!', // WARNING
		'&'  // DEBUG 
	};

	void message(MType t_type, const char* t_msg, ...);

}
