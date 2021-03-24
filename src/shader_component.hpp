#pragma once
#include <cstdint>
#include <string>


namespace vpanic {

	struct ShaderComponent {
	
		void compile(const int t_type);	
		int type { 0 };
		uint32_t id { 0 };
		std::string src { "" };

	};

}
