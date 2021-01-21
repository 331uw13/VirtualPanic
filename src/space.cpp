#include "space.hpp"

namespace VPanic {

	vec2 Space::xy() const {
		return vec2(x, y);
	}

	vec2 Space::wh() const {
		return vec2(w, h);
	}

	bool Space::empty() const {
		return (x == 0 && y == 0 && w == 0 && h == 0);
	}

}


