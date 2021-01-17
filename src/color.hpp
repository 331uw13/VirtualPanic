#pragma once

namespace VPanic {

	struct Color {
	
		uint32_t r, g, b, a;

		Color(uint32_t t_r, uint32_t t_g, uint32_t t_b, uint32_t t_a = 255) : 
			r(t_r), g(t_g), b(t_b), a(t_a) {}
		
		~Color() {};
	};

}
