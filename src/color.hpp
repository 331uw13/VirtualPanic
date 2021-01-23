#pragma once
#include <stdint.h>

namespace VPanic {

	struct Color {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a = 255)
			: r(t_r), g(t_g), b(t_b), a(t_a) {}

		Color(uint8_t t_rgb, uint8_t t_a = 255)
			: r(t_rgb), g(t_rgb), b(t_rgb), a(t_a) {}
	
		Color() : r(0), g(0), b(0), a(255) {}
	};
}
