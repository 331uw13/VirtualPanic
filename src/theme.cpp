#include <fstream>
#include <algorithm>

#include "theme.hpp"
#include "messages.hpp"
#include "utils.hpp"

namespace vpanic {

	Theme::Theme(const char* t_filename) {
		read(t_filename);
	}

	bool Theme::read(const char* t_filename) {

		std::fstream f(t_filename);
		if(!f.is_open()) {
			message(MSG_ERROR, "Cannot open file \"%s\"", t_filename);
			return false;
		}

		std::string line;
		while(getline(f, line)) {
			if(line.find(0x22) == std::string::npos) {
				continue;
			}

			const uint32_t label_start = line.find(0x22)+1;
			const uint32_t label_end = line.find_last_of(0x22)-1;
			
			const uint32_t value_start = line.find(0x28)+1;
			const uint32_t value_end = line.find(0x29)-value_start;

			const std::string label = line.substr(label_start, label_end);
			std::string value = line.substr(value_start, value_end);

			std::transform(value.begin(), value.end(), value.begin(), 
					[](const uint8_t c) { return std::toupper(c); });

			if(!is_hex_string(value)) { continue; }

			const uint32_t hex = std::stoi(value, nullptr, 16);
			const uint8_t red    = hex/0x10000;
			const uint8_t green  = (hex/0x100)%0x100;
			const uint8_t blue   = hex%0x100;
			
			m_table[label] = Color(red, green, blue);
		}


		f.close();
		return true;
	}

	/*
	bool Theme::_test(const std::string& t_str) {
		return (t_str.find_first_not_of("X0123456789ABCDEF") == std::string::npos) && !t_str.empty();
	}
	*/

	void Theme::clear() {
		m_table.clear();
	}
	
	Color Theme::get(const char* t_name) {
		return m_table[t_name];
	}

}
