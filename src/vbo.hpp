#pragma once
#include <cstdint>

// Vertex Buffer Object

namespace VPanic {
	
	class VBO {
	public:

		~VBO();
	
		void data(const float* t_data);

		void bind();
		void unbind();
		
		void gen();
		void del();

	private:
		uint32_t m_id { 0 };
	};
}

