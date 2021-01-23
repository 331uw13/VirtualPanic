#pragma once

namespace VPanic {

	struct Vec {
		
		float x;
		float y;

		Vec(const float t_x, const float t_y) 
			: x(t_x), y(t_y) {}

		Vec(const float t_xy)
			: x(t_xy), y(t_xy) {}

		Vec() 
			: x(0), y(0) {}


		Vec operator / (const Vec& t);
		Vec operator / (const float t);

		Vec operator - (const Vec& t);


	};

}
