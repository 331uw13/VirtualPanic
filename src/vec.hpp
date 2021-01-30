#pragma once

namespace VPanic {

	struct vec2 {
		
		float x;
		float y;

		vec2(const float t_x, const float t_y) 
			: x(t_x), y(t_y) {}

		vec2(const float t_xy)
			: x(t_xy), y(t_xy) {}

		vec2() 
			: x(0), y(0) {}


		vec2 operator / (const vec2& t);
		vec2 operator / (const float t);

		vec2 operator - (const vec2& t);
	};

	struct vec3 {
		
		float x;
		float y;
		float z;

		vec3(const float t_x, const float t_y, const float t_z)
			: x(t_x), y(t_y), z(t_z) {}
		
		vec3(const float t_xyz)
			: x(t_xyz), y(t_xyz), z(t_xyz) {}
		
		vec3() : x(0), y(0), z(0) {}

	};

}
