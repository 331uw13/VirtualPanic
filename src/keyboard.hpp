#pragma once

namespace VPanic {

	namespace Keyboard {
	
		enum class Mod {
			CTRL = 64,
			SHIFT = 1,
			ALT = 256,
			SPACE = -1,
		};

		bool keydown(const uint8_t t_key);
		bool keydown(Mod t_mod);

	}

}
