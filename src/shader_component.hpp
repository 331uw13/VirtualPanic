#pragma once
#include <cstdint>
#include <string>

#include "shader_type.hpp"


namespace vpanic {

	struct ShaderComponent {
	
		void compile(const int t_type);	
		int type { INVALID_SHADER };
		uint32_t id { 0 };
		std::string src { "" };

	};

}
