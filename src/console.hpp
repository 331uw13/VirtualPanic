#pragma once
#include <vector>
#include <string>

#include "color.hpp"


namespace vpanic {

	class Console {
	public:

		static Console& instance() {
			static Console console;
			return console;
		}

		void focus();
		void update();
		void clear_text();
		void print(const char* t_text, ...);
		void print(const Color& t_color, const char* t_text, ...);
		void set_callback(void(*t_callback)(const std::vector<std::string>&));
		void set_font_size(const float t_size);
		float get_font_size() const;

		Console(const Console&) = delete;

	private:
	
		Console() {}

		float m_font_size { 1.10f };
		char m_input_buffer[600];
		std::vector<std::pair<const char*, Color>> m_text;
		void(*m_callback)(const std::vector<std::string>&) { nullptr };
		bool m_input_needs_focus { false };
		
		void _exec(const char* t_cmd);
	};

}

