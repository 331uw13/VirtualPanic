#include <numeric>
#include "libs/gl3w.h"

#include "uniform_buffer.hpp"


namespace vpanic {


	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &m_id);
		m_id = 0;
		m_max_size = 0;
		m_created = false;
	}

	void UniformBuffer::create(const std::vector<uint32_t>& t_size_layout) {
		const uint32_t size = std::accumulate(t_size_layout.begin(), t_size_layout.end(), 0);
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_id, 0, size);
		m_max_size = size;
		m_created = true;
		
		m_layout = t_size_layout;

		// set first index to 0, because glBufferSubData wants 'offset' not 'index'
		m_layout.insert(m_layout.begin(), 0);
	}

	void UniformBuffer::set(const uint32_t t_index, void* t_ptr) {
		if(!m_created) { return; }
		if(t_index >= m_layout.size()) { return; }
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		const uint32_t offset = m_layout[t_index]*t_index;
		const uint32_t size = m_layout[t_index+1];
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, t_ptr);
	}


}


