#pragma once
#include <vector>

#include "shader.hpp"


namespace vpanic {

	class UniformBuffer {
	public:

		UniformBuffer() {}
		~UniformBuffer();

		void create(const std::vector<uint32_t>& t_size_layout);
		void set(const uint32_t t_index, void* t_ptr);

	private:

		std::vector<uint32_t> m_layout;
		size_t m_max_size  { 0 };
		uint32_t m_id      { 0 };
		bool m_created     { false };

	};

}
