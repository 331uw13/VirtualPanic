#pragma once

namespace VPanic {

	struct Color {
	
		uint8_t r, g, b, a;


		Color(uint8_t t_r, uint8_t t_g, uint8_t t_b, uint8_t t_a = 255) : 
			r(t_r), g(t_g), b(t_b), a(t_a) {}
		
		Color() {
			r = g = b = 0;
			a = 255;
		}
		
		~Color() {};
	};

}
