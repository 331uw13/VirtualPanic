#pragma once

namespace VPanic {	
	enum class MsgType {
		INFO,
		ERROR,
		WARNING,
		SUCCESS,
		DEBUG
	};

	const unsigned char m_colors[5] {
		0,   // Info
		91,  // Error
		93,  // Warning
		92,  // Success
		35   // Debug
	};

	const unsigned char m_visual[5] {
		'i',  // Info
		'x',  // Error
		'!',  // Warning
		'+',  // Success
		'@'   // Debug  
	};

	const char m_prefix[] { " (VirtualPanic)" };

	void message(const char* t_text, MsgType t_type = MsgType::INFO);
	void message(const char* t_text, const char* t_text2, MsgType t_type = MsgType::INFO);

}

