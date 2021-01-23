#include "vec.hpp"

namespace VPanic {

	Vec Vec::operator / (const Vec& t) {
		return Vec(x / (t.x == 0) ? 1 : t.x, y / (t.y == 0) ? 1 : t.y);
	}

	Vec Vec::operator / (const float t) {
		const float d = (t == 0) ? 1 : t;
		return Vec(x / d, y / d);
	}

	Vec Vec::operator - (const Vec& t) {
		return Vec(x - t.x, y - t.y);
	}


}
