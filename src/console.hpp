#pragma once

#include "color.hpp"
#include "state.hpp"


namespace vpanic {

	namespace console {

		void render();
		void clear();
		
		ConsoleState copy_state();
		ConsoleState& state();

		void print(const char* t_fmt, ...);

	}


	/*
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
		void callback(void(*t_callback)(const std::vector<std::string>&));
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
	*/

}

