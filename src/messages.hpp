#pragma once
#include "color.hpp"

namespace vpanic {

	enum class MType { // "message type"
		INFO,
		OK,
		ERROR,
		SHADER_ERROR,
		WARNING,
		SHADER_WARNING,
		DEBUG
	};

	static const Color _msg_colors[8] {
		Color(50, 50, 200), // INFO
		Color(45, 230, 45), // OK
		Color(230, 45, 45), // ERROR
		Color(230, 45, 45), // SHADER ERROR
		Color(230, 140, 23), // WARNING
		Color(230, 140, 23), // SHADER WARNING
		Color(200, 45, 200), // DEBUG
	};

	static const char* const _msg_types[8] {
		"INFO",
		"OK",
		"ERROR",
		"SHADER_ERROR",
		"WARNING",
		"SHADER_WARNING",
		"DEBUG"
	};

	void message(MType t_type, const char* t_msg, ...);

}
