#pragma once
#include <cstdint>

// Index Buffer Object

namespace VPanic {
	
	class IBO {
	public:

		~IBO();
	
		void data(const uint32_t* t_indices);

		void bind();
		void unbind();
		
		void gen();
		void del();

		uint32_t index_count() const;

	private:
		uint32_t m_index_count  { 0 };
		uint32_t m_id           { 0 };
	};
}

