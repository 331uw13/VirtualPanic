#pragma once
#include "color.hpp"
#include "vec.hpp"

namespace VPanic {

	struct Rect {
		float x, y, w, h;
		Color color;

		Rect(const Vec& t_pos, const Vec& t_size, const Color& t_color = Color())
			: x(t_pos.x), y(t_pos.y), w(t_size.x), h(t_size.y), color(t_color) {}
		
		Rect(const float t_x, const float t_y, const float t_w, const float t_h, const Color& t_color = Color())
			: x(t_x), y(t_y), w(t_w), h(t_h), color(t_color) {}

		Rect() : x(0), y(0), w(0), h(0), color(Color()) {}
	};

	struct Triangle {
		Vec points[3];
		Color color;

		Triangle(const Vec& t_point0, const Vec& t_point1, const Vec& t_point2, const Color& t_color = Color()) {
			points[0] = t_point0;
			points[1] = t_point1;
			points[2] = t_point2;
			color = Color();
		}

		Triangle() {
			points[0] = points[1] = points[2] = Vec();
			color = Color();
		}

	};



}
