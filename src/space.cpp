#include "space.hpp"

namespace VPanic {

	vec2 Space::xy() const {
		return vec2(x, y);
	}

	vec2 Space::wh() const {
		return vec2(w, h);
	}

<<<<<<< HEAD
	bool Space::empty() const {
		return (x <= 0 && y <= 0 && w <= 0 && h <= 0);
	}

=======
>>>>>>> 5dde9358b014d1274ef2f7b1cede503248eb90ce
}


