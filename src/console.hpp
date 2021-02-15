#pragma once
#include <vector>
#include "color.hpp"


namespace vpanic {

	class Console {
	public:

		void update();	
		void add(const char* t_text, ...);
		void add(const char* t_text, const Color& t_color, ...);

	private:

		std::vector<std::pair<const char*, Color>> m_text;

	};

}

