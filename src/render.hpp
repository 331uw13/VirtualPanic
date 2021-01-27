#pragma once
#include <vector>

#include "shapes.hpp"
#include "vbo.hpp"
#include "ibo.hpp"
#include "vec.hpp"

namespace VPanic {
	
	class Render {
	public:
		
		void init();
		void draw_rect(const Rect& t_rect);

	private:

		uint32_t m_vao { 0 };
		VBO m_vbo;
		IBO m_ibo;
		
		std::vector<Rect> m_rects;
	};
}







