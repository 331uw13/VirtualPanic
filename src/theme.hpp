#include <cstdio>
#include <string>
#include <unordered_map>

#include "color.hpp"

namespace vpanic {
	
	class Theme {
	public:

		Theme(const char* t_filename);
		Theme() {}

		bool read(const char* t_filename);
		void clear();

		Color get(const char* t_name);
	
	private:

		std::unordered_map<std::string, Color> m_table;

	};

}
