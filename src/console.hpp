#pragma once
#include <vector>
#include <string>

#include "color.hpp"
#include "state.hpp"


namespace vpanic {

	namespace console {

		void render();
		void clear();
		void add_command(const char* t_str, void(*t_callback)(const std::vector<std::string>&));

		State copy_state();
		State& state();

		void print(const char* t_fmt, ...);

	}

}

